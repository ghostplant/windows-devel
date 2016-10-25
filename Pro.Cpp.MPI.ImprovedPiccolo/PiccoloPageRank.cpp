#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include <mpi.h>
#include <unistd.h>
#include <sys/stat.h>

#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>


using namespace std;

#define GRAPH_PATH "all.input_graph/web-Stanford_v281903_e2312497.txt"

// v1 --
//
//   二层分组（图顶点->进程）、
//   纯分布式加载图、
//   纯分布式保存结果、
//   读写锁与事件锁、
//   PageRank基本算法

// v2 --
//
//    改为三层分组（图顶点->组->进程）
//    本地单个MapPartition扩展为多MapPartitions（PARTITION_SIZE决定）
//    透明化多MapPartitions的迭代器（it.next()/it.data()）

// v3 -- 弱一致性优化、MARKER标记同步
//
//    针对 updateValue，为每个目标节点设置长度为 WEAK_BUFFER 的发送缓存，延迟发送直到缓存满
//    通过 flush_group(worker) 指定触发单个目标worker的所有未发送缓存
//    用户代码部分：需要在大同步前 next->flush() 来触发所有未发送缓存
//    收集总节点数量的 Markers 完成一次同步操作

// v4 -- 分布式快照算法、检查点恢复\graph_degree\master_log
//
//    利用Chandy-Lamport算法在每个机器大不同步时发起Marker通知
//    利用原子文件操作保证旧日志、新日志替换时刻的可靠性
//    读取快照进行分布地Restore过程，回复局部Table数据和用户自定义的meta数据
//    输出图节点的度分布
//    将调试性Log信息进行了封装
//    精确化统计时间

// v5 -- 内存与硬盘的键值对数据交换
//
//    每个分布式Tables的加入局部总size信息
//    设置总size上限，超过上限则拉取部分键值对到硬盘
//    读取被加载到硬盘的数据时，将再次加载到内存，置换其他键值对到硬盘: create() & swap_out() ; swap_out() & swap_in(want)


#define IN_MEMORY_KEY_LIMIT 4000000
#define CHECKPOINT_INTERVAL	300  // PageRank 执行检查点的频度
#define PAGERANK_TOT_STEP	3  // PageRank 总迭代次数

//#define DIRECTED_GRAPH // 决定是否按照有向图方式（正、反边添加两次）加载网络

#define USE_WEAK_CONSISTENCY // 决定是否采用弱一致性优化
#define WEAK_BUFFER	65536 // 调整缓冲区大小（byte）

#define MAX_WORKER_NUM	8 // 工作节点最大上限数量，更多实际节点请调大参数
#define PARTITION_SIZE	256
#define BUFFER_UNIT		2000
#define MAX_MSG_LENGTH	(1<<24)

//==================== 读写锁部分

#define EMPTY_LOCK	0
#define READ_LOCK	1
#define WRITE_LOCK	2

class ReadWriteMutex {
	
protected:
	int count, status;
	mutex *rmtx, *wmtx;
public:
	ReadWriteMutex() {
		count = 0, status = EMPTY_LOCK;
		rmtx = new mutex(), wmtx = new mutex();
	}
	
	~ReadWriteMutex() {
		delete rmtx;
		delete wmtx;
	}
	
	void read_lock() {
		rmtx->lock();
		if (++count==1)
			wmtx->lock(), status = READ_LOCK;
		rmtx->unlock();
	}
	
	void write_lock() {
		wmtx->lock(), status = WRITE_LOCK;
	}
	
	void unlock() {
		if (status == READ_LOCK) {
			rmtx->lock();
			if (--count==0)
				status = EMPTY_LOCK, wmtx->unlock();
			rmtx->unlock();
		} else if (status == WRITE_LOCK)
			status = EMPTY_LOCK, wmtx->unlock();
	}
};

//==================== 事件锁部分

class EventMutex {
	
protected:
	mutex *event;
	void *data;
public:
	EventMutex() {
		event = new mutex();
		event->lock();
		data = NULL;
	}
	
	~EventMutex() {
		delete event;
	}
	
	bool wait() {
		event->lock();
		return data != NULL;
	}
	void* &buffer() {
		return data;
	}
	
	void signal() {
		event->unlock();
	}
};

//==================== Key/Value 数据表全局信息

#define P2P_DO_UPDATE	0x1
#define P2P_DO_SETVAL	0x2
#define P2P_DO_GETVAL	0x3
#define P2P_DO_RESVAL	0x4
#define P2P_DO_SHUTDOWN	0x5

#define P2P_DO_UPDATEBUF 0x6
#define P2P_DO_MARKER	0x7

class DistriSharedMap;

static struct peer_status {
	unsigned short uuid_allocator;
	unordered_map<unsigned short, DistriSharedMap*> table_map;
	int rank, total, marker_counter, log_timestamp;
	EventMutex get_value_event, sync_event;
	
	thread *mail_listener;
} __peer_status;


//==================== 通用数据项操作（元数据，序列化数据）

struct metadata {
	unsigned short op;
	unsigned short uuid;
	char data[];
};

struct serializable_data {
	int size, alloc;
	char data[];
};

serializable_data* create_serializable_data() {
	serializable_data* data = (serializable_data*)malloc(sizeof(serializable_data) + BUFFER_UNIT);
	data->size = 0;
	data->alloc = BUFFER_UNIT;
	*((long*)data->data) = 0;
	return data;
}

serializable_data* clone_serializable_data(const serializable_data *src) {
	serializable_data* cpy = (serializable_data*)malloc(sizeof(serializable_data) + src->alloc);
	memcpy(cpy, src, sizeof(serializable_data) + src->size);
	return cpy;
}

void destory_serializable_data(serializable_data *&data) {
	free(data);
	data = NULL;
}

//==================== 单个Key/Value数据表结构

void P2P_Synchronize(unsigned int table_cnt = 0, DistriSharedMap *tables = NULL, string serial = "");

void P2P_Master_Log(const string &s) {
	if (__peer_status.rank == 0)
		printf("%s\n", s.c_str());
}

void P2P_Node_Log(const string &s) {
	printf("[%d] %s\n", __peer_status.rank, s.c_str());
}

class DistriSharedMap {
public:
	typedef unordered_map<const void*, serializable_data*> table_t;
	
protected:
	unsigned short uuid;
	table_t localMap[PARTITION_SIZE];
	int partition_groups[PARTITION_SIZE];
	
	char weak_buffer[MAX_WORKER_NUM][WEAK_BUFFER];
	int weak_length[MAX_WORKER_NUM];
	
	// 缓存数据
	void bufferValue(const void *key, const void* incValue, int g) {
		if (weak_length[g] + sizeof(metadata)+sizeof(void*)+sizeof(void*) > WEAK_BUFFER-sizeof(metadata))
			flush_group(g);
		
		metadata md = {P2P_DO_UPDATEBUF, uuid};
		
		memcpy(weak_buffer[g]+weak_length[g], &md, sizeof(metadata)); // metadata
		memcpy(weak_buffer[g]+weak_length[g]+sizeof(metadata), &key, sizeof(void*)); // key
		memcpy(weak_buffer[g]+weak_length[g]+sizeof(metadata)+sizeof(void*), &incValue, sizeof(void*)); // incValue
		
		weak_length[g]+=sizeof(metadata)+sizeof(void*)+sizeof(void*);
	}

	void flush_group(int g) {
		if (!weak_length[g])
			return;
		
		static metadata md = {0, 0};
		assert(weak_length[g]+sizeof(metadata) <= WEAK_BUFFER);
		memcpy(weak_buffer[g]+weak_length[g], &md, sizeof(metadata)); // metadata
		weak_length[g]+=sizeof(metadata);
		
		MPI_Send(weak_buffer[g],weak_length[g],MPI_CHAR,g,0,MPI_COMM_WORLD);
		weak_length[g] = 0;
	}
	
public:
	ReadWriteMutex rwmtx;
	
	int inMemoryKeys(bool lock = false) {
		if (lock)
			rwmtx.write_lock();
		int tot=0;
		for (int i=0;i<PARTITION_SIZE;i++)
			tot+=localMap[i].size();
		if (lock)
			rwmtx.unlock();
		return tot;
	}
	
	void swapIn(int partition) {
		if (disk_swap[partition]==-1)
			return;
		assert(localMap[partition].size()==0);
		
		FILE *fp=fopen(("dsm._swap_files/data-"+to_string(__peer_status.rank)+"-"+to_string(uuid)+"-"+to_string(partition)+".swp").c_str(), "rb");
		assert(fp!=NULL);

		void *key; int ret;
		static char buffer[MAX_MSG_LENGTH];
		for (int i=disk_swap[partition];i;i--) {
			ret = fread(&key, 1, sizeof(key), fp);
			ret = fread(buffer, 1, sizeof(serializable_data), fp);
			ret = fread(buffer+sizeof(serializable_data), 1, ((serializable_data*)buffer)->size, fp);
			//printf("@@@@ %d-%d-%d %ld %d\n", __peer_status.rank, uuid, partition, key, ((serializable_data*)buffer)->size);
			assert(localMap[partition].find(key) == localMap[partition].end());
			localMap[partition][key] = clone_serializable_data((serializable_data*)buffer);
		}
		fclose(fp);
	
		assert(localMap[partition].size() == disk_swap[partition]);
		//__peer_status.nok += __peer_status.table_map[uuid]->in_disk_size[partition];
		disk_swap[partition]=-1;
	
		P2P_Node_Log("table-partition ("+to_string(uuid)+", "+to_string(partition)+") was swapped in.");
	}
	
	void swapOut() {
		//rwmtx.write_lock();
		
		int len=-1, partition;
		for (int i=0;i<PARTITION_SIZE;i++) {
			if (~disk_swap[i])
				continue;
			if (len<(int)localMap[i].size())
				len=localMap[i].size(), partition=i;
		}
		if (len<=0)
			return;
		assert(localMap[partition].size()==len);
		assert(disk_swap[partition]==-1);
		//it.second->p2p_iterator();
		
		mkdir("dsm._swap_files/", 0775);
		FILE *fp=fopen(("dsm._swap_files/data-"+to_string(__peer_status.rank)+"-"+to_string(uuid)+"-"+to_string(partition)+".swp").c_str(), "wb");
		for (auto it: localMap[partition]) {
			const void *key=it.first;
			fwrite(&key, 1, sizeof(key), fp);
			fwrite(it.second, 1, sizeof(serializable_data)+it.second->size, fp);
		}
		fclose(fp);
	
		for (auto it: localMap[partition])
			free(it.second);
		localMap[partition].clear();
		disk_swap[partition] = len;
		
		//__peer_status.table_map[uuid]->in_disk_size[partition] = len;
		//__peer_status.nok -= len;
	
		P2P_Node_Log("table-partition ("+to_string(uuid)+", "+to_string(partition)+") was swapped out.");
		// __peer_status.rwmtx.unlock();
		
		//rwmtx.unlock();
		
	}
	
	int disk_swap[PARTITION_SIZE];
	bool swap;

	DistriSharedMap(bool swap = false) {
		uuid = __peer_status.uuid_allocator++;
		__peer_status.table_map[uuid] = this;
		memset(weak_length, 0, sizeof(weak_length));
		this->swap = swap;
		clear();
	}
	
	~DistriSharedMap() {
		__peer_status.table_map.erase(uuid);
	}
	
	void flush() {
		for (int i=0;i<__peer_status.total;i++)
			flush_group(i);
	}
	
	void clear() {
		for (int i=0;i<MAX_WORKER_NUM;i++)
			assert(weak_length[i] == 0);
		for (int i=0;i<PARTITION_SIZE;i++) {
			for (auto it: localMap[i])
				free(it.second);
			localMap[i].clear();
			partition_groups[i] = i % __peer_status.total;
			disk_swap[i] = -1;
		}
	}
	
	struct key_iterator {
		int at;
		unsigned int g_size;
		table_t* groups;
		table_t::iterator g_iter;
		
		bool next() {
			if (at<0)
				at=0, g_iter=groups[at].begin();
			else
				g_iter++;
			while (g_iter==groups[at].end() && at+1<PARTITION_SIZE)
				g_iter=groups[++at].begin();
			return (g_iter!=groups[at].end());
		}
		
		const table_t::iterator data() {
			return g_iter;
		}
		
		unsigned int size() {
			return g_size;
		}
	};
	
	const serializable_data* getValue(const void* key, const serializable_data* def) {
		int hk = partition(key), part = group_of(hk);
		if (part == __peer_status.rank) {
			rwmtx.write_lock();
			swapIn(hk);
			auto it = localMap[hk].find(key);
			auto ans = (it != localMap[hk].end())?it->second:def;
			rwmtx.unlock();
			return ans;
		} else {
			metadata md = {P2P_DO_GETVAL, uuid};
			unsigned int buflen = sizeof(metadata)+sizeof(void*);
			static char buf[MAX_MSG_LENGTH], res[MAX_MSG_LENGTH];
			memcpy(buf, &md, sizeof(metadata)); // metadata
			memcpy(buf+sizeof(metadata), &key, sizeof(void*)); // key
			
			__peer_status.get_value_event.buffer() = res;
			MPI_Send(buf,buflen,MPI_CHAR,part,0,MPI_COMM_WORLD);
			if (!__peer_status.get_value_event.wait())
				return def;
			return (serializable_data*)buf;
		}
	}
	
	void setValue(const void *key, const serializable_data* value) {
		int hk = partition(key), part = group_of(hk);
		if (part == __peer_status.rank) {
			rwmtx.write_lock();
			auto it = localMap[hk].find(key);
			if (it != localMap[hk].end())
				free(it->second);
			localMap[hk][key] = clone_serializable_data(value);
			rwmtx.unlock();
		} else {
			metadata md = {P2P_DO_SETVAL, uuid};
			unsigned int buflen = sizeof(metadata)+sizeof(void*)+sizeof(serializable_data)+value->size;
			assert(buflen < MAX_MSG_LENGTH);
			static char buf[MAX_MSG_LENGTH];
			memcpy(buf, &md, sizeof(metadata)); // metadata
			memcpy(buf+sizeof(metadata), &key, sizeof(void*)); // key
			memcpy(buf+sizeof(metadata)+sizeof(void*), value, sizeof(serializable_data)+value->size); // values
			MPI_Send(buf,buflen,MPI_CHAR,part,0,MPI_COMM_WORLD);
		}
	}
	
	void updateValue(const void *key, const void* incValue) {
		int hk = partition(key), part = group_of(hk);
		if (part == __peer_status.rank) {
			rwmtx.write_lock();
			
			swapIn(hk);
			
			auto it = localMap[hk].find(key);
			if (it == localMap[hk].end()) {
				auto data = create_serializable_data();
				localMap[hk][key] = data;
				combineValue(data, incValue);
			} else
				combineValue(it->second, incValue);
			
			while (swap && inMemoryKeys() > IN_MEMORY_KEY_LIMIT) {
				swapOut();
			}
			
			/*int tot=0;
			for (int i=0;i<PARTITION_SIZE;i++)
				tot+=localMap[i].size();
			printf("============ %lu %d %d\n", key, tot, __peer_status.rank);*/
			rwmtx.unlock();
		} else {
#ifdef USE_WEAK_CONSISTENCY
			bufferValue(key, incValue, part);
			//flush_group(part);
#else
			metadata md = {P2P_DO_UPDATE, uuid};
			unsigned int buflen = sizeof(metadata)+sizeof(void*)+sizeof(void*);
			static char buf[MAX_MSG_LENGTH];
			memcpy(buf, &md, sizeof(metadata)); // metadata
			memcpy(buf+sizeof(metadata), &key, sizeof(void*)); // key
			memcpy(buf+sizeof(metadata)+sizeof(void*), &incValue, sizeof(incValue)); // incValue
			MPI_Send(buf,buflen,MPI_CHAR,part,0,MPI_COMM_WORLD);
#endif
		}
	}
	
	const key_iterator p2p_iterator() {
		unsigned int tot=0;
		for (int i=0;i<PARTITION_SIZE;i++) {
			swapIn(i);
			tot+=localMap[i].size();
		}
		//printf("[[%d %d]]\n", tot, __peer_status.rank);
		key_iterator it = {-1, tot, localMap};
		return it;
	}
	
	int group_of(int partition) {
		return partition_groups[partition];
	}
	
	void saveAsTextFile(const char *path, string (*f)(const table_t::iterator&)) {
		mkdir(path, 0775);
		static char buffer[BUFFER_UNIT];
		sprintf(buffer, "%s/part-%05d", path, __peer_status.rank);
		FILE *fp=fopen(buffer, "w");
		auto it = p2p_iterator();
		while (it.next())
			fprintf(fp, "%s\n", f(it.data()).c_str());
		fclose(fp);
		P2P_Synchronize();
		if (!__peer_status.rank) {
			sprintf(buffer, "%s/_SUCCESS", path);
			FILE *fp=fopen(buffer, "w");
			fprintf(fp, "%d", __peer_status.total);
			fclose(fp);
		}
	}
	
	virtual int partition(const void *key) {
		return (*(unsigned long*)&key) % PARTITION_SIZE;
	}
	
	virtual void combineValue(serializable_data *&value, const void *incValue) = 0;
};


/*int P2P_Number_of_Keys() {
	int tot=0;
	for (auto it: __peer_status.table_map)
		for (int i=0;i<PARTITION_SIZE;i++)
			tot+=it.second->localMap[i].size();
	return tot;
}

void P2P_Swap_Out() {
	// __peer_status.rwmtx.write_lock();
	
	int len=0, uuid, partition;
	for (auto it: __peer_status.table_map) {
		for (int i=0;i<PARTITION_SIZE;i++) {
			if (len<it.second->localMap[i].size())
				len=it.second->localMap[i].size(), uuid=it.first, partition=i;
		}
		//it.second->p2p_iterator();
	}
	if (len<=0)
		return;
	assert(__peer_status.table_map[uuid]->localMap[partition].size()==len);
	//printf("!!!!! %d\n", __peer_status.table_map[uuid]->in_disk_size[partition]);
	//fflush(stdout);
	assert(__peer_status.table_map[uuid]->in_disk_size[partition]==-1);
	
	FILE *fp=fopen(("dsm._swap_files/data-"+to_string(__peer_status.rank)+"-"+to_string(uuid)+"-"+to_string(partition)+".swp").c_str(), "wb");
	for (auto it: __peer_status.table_map[uuid]->localMap[partition]) {
		const void *key=it.first;
		fwrite(&key, 1, sizeof(key), fp);
		fwrite(it.second, 1, sizeof(serializable_data)+it.second->size, fp);
	}
	fclose(fp);
	
	for (auto it: __peer_status.table_map[uuid]->localMap[partition])
		free(it.second);
	__peer_status.table_map[uuid]->localMap[partition].clear();
	//__peer_status.table_map[uuid]->in_disk_size[partition] = len;
	//__peer_status.nok -= len;
	
	//P2P_Node_Log("table-partition ("+to_string(uuid)+", "+to_string(partition)+") was swapped out.");
	// __peer_status.rwmtx.unlock();
}

void P2P_Swap_In(int uuid, int partition) {
	//__peer_status.table_map[uuid]->rwmtx.write_lock();
	
	if (__peer_status.table_map[uuid]->in_disk_size[partition]==-1)
		return;
	
	assert(__peer_status.table_map[uuid]->localMap[partition].size()==0);
	
	while (P2P_Number_of_Keys() > LOCAL_IN_MEMORY_KEY_LIMIT - __peer_status.table_map[uuid]->in_disk_size[partition] - 1) {
		int tot=0;
		for (auto it: __peer_status.table_map)
			for (int i=0;i<PARTITION_SIZE;i++)
				tot+=it.second->localMap[i].size();
		printf("+++%d %d %d\n", __peer_status.rank, __peer_status.nok, tot);
		
		P2P_Swap_Out();
		
		tot=0;
		for (auto it: __peer_status.table_map)
			for (int i=0;i<PARTITION_SIZE;i++)
				tot+=it.second->localMap[i].size();
		printf("---%d %d %d\n", __peer_status.rank, __peer_status.nok, tot);
		sleep(1);
	}
	
	FILE *fp=fopen(("dsm._swap_files/data-"+to_string(__peer_status.rank)+"-"+to_string(uuid)+"-"+to_string(partition)+".swp").c_str(), "rb");
	assert(fp!=NULL);

	void *key; int ret;
	static char buffer[MAX_MSG_LENGTH];
	for (int i=__peer_status.table_map[uuid]->in_disk_size[partition];i;i--) {
		ret = fread(&key, 1, sizeof(key), fp);
		ret = fread(buffer, 1, sizeof(serializable_data), fp);
		ret = fread(buffer+sizeof(serializable_data), 1, ((serializable_data*)buffer)->size, fp);
		//printf("@@@@ %d-%d-%d %ld %d\n", __peer_status.rank, uuid, partition, key, ((serializable_data*)buffer)->size);
		assert(__peer_status.table_map[uuid]->localMap[partition].find(key)==__peer_status.table_map[uuid]->localMap[partition].end());
		__peer_status.table_map[uuid]->localMap[partition][key] = clone_serializable_data((serializable_data*)buffer);
	}
	fclose(fp);
	
	assert(__peer_status.table_map[uuid]->localMap[partition].size() == __peer_status.table_map[uuid]->in_disk_size[partition]);
	//__peer_status.nok += __peer_status.table_map[uuid]->in_disk_size[partition];
	__peer_status.table_map[uuid]->in_disk_size[partition]=-1;
	
	P2P_Node_Log("table-partition ("+to_string(uuid)+", "+to_string(partition)+") was swapped in.");
	//__peer_status.table_map[uuid]->rwmtx.unlock();
}*/

//==================== Epoll式并发消息接收通道、框架初始化、框架资源回收

static void P2P_Recv_Listener() {
	static char buf[MAX_MSG_LENGTH];
	
	MPI_Status sta;
	while (MPI_Recv(buf,sizeof(buf),MPI_CHAR,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&sta), ((metadata*)buf)->op != P2P_DO_SHUTDOWN) {
		switch (((metadata*)buf)->op) {
			case P2P_DO_UPDATEBUF: {
				for (char *p=buf; ((metadata*)p)->op; p+=sizeof(metadata)+sizeof(void*)+sizeof(void*)) {
					assert(P2P_DO_UPDATEBUF == ((metadata*)p)->op);
					auto it = __peer_status.table_map.find(((metadata*)p)->uuid);
					assert(it != __peer_status.table_map.end());
					auto data = ((metadata*)p)->data;
				
					const void *key = *(void**)data;
					const void *incValue = *(void**)(data+sizeof(void*));
					auto dsm = it->second;
					assert(dsm->group_of(dsm->partition(key)) == __peer_status.rank);
					dsm->updateValue(key, incValue);
				}
				break;
			}
			case P2P_DO_UPDATE: {
				auto it = __peer_status.table_map.find(((metadata*)buf)->uuid);
				//printf("Get length %d %d %d\n", sta.count_lo, ((metadata*)buf), ((metadata*)buf)->data);
				assert(it != __peer_status.table_map.end());
				auto data = ((metadata*)buf)->data;
				
				const void *key = *(void**)data;
				const void *incValue = *(void**)(data+sizeof(void*));
				//printf("@@@@ %ld %ld\n", key, incValue);
				auto dsm = it->second;
				assert(dsm->group_of(dsm->partition(key)) == __peer_status.rank);
				dsm->updateValue(key, incValue);
				break;
			}
			case P2P_DO_SETVAL: {
				auto it = __peer_status.table_map.find(((metadata*)buf)->uuid);
				assert(it != __peer_status.table_map.end());
				auto data = ((metadata*)buf)->data;
				auto key = *(void**)data;
				auto values = (serializable_data *)(data+sizeof(void*));
				auto dsm = it->second;
				assert(dsm->group_of(dsm->partition(key)) == __peer_status.rank);
				dsm->setValue(key, values);
				break;
			}
			case P2P_DO_GETVAL: {
				auto it = __peer_status.table_map.find(((metadata*)buf)->uuid);
				assert(it != __peer_status.table_map.end());
				auto data = ((metadata*)buf)->data;
				auto key = *(void**)data;
				auto dsm = it->second;
				assert(dsm->group_of(dsm->partition(key)) == __peer_status.rank);
				auto value = dsm->getValue(key, NULL);
				
				((metadata*)buf)->op = P2P_DO_RESVAL;
				unsigned int buflen = sizeof(metadata)+sizeof(void*);
				unsigned long long nothing = 0;
				if (value != NULL) {
					memcpy(buf+buflen, value, sizeof(serializable_data)+value->size);
					buflen += sizeof(serializable_data)+value->size;
				} else {
					memcpy(buf+buflen, &nothing, sizeof(nothing));
					buflen += sizeof(nothing);
				}
				MPI_Send(buf,buflen,MPI_CHAR,sta.MPI_SOURCE,0,MPI_COMM_WORLD);
				break;
			}
			case P2P_DO_RESVAL: {
				auto data = (serializable_data*)((metadata*)buf)->data;
				if (data->alloc == 0)
					__peer_status.get_value_event.buffer() = NULL;
				else {
					assert(sizeof(serializable_data)+data->size < MAX_MSG_LENGTH);
					memcpy(__peer_status.get_value_event.buffer(), data, sizeof(serializable_data)+data->size);
				}
				__peer_status.get_value_event.signal();
				break;
			}
			case P2P_DO_MARKER: {
				static int marker_count = 0;
				if (++marker_count == __peer_status.total) {
					marker_count = 0;
					__peer_status.sync_event.signal();
				}
				break;
			}
			default:
				fprintf(stderr, "Unsupported operation by peer %d!\n", __peer_status.rank);
		}
	}
}


int P2P_Try_Restore(DistriSharedMap *tables, string &meta) {
	FILE *fp=fopen("dsm._checkpoint/" "journal", "rb");
	if (!fp)
		return -1;
	unsigned int mac_size, table_size, meta_size, byte = 0, ret, local_timestamp;
	string local_meta = "";
	
	ret = fread(&local_timestamp, 1, sizeof(local_timestamp), fp);
	ret = fread(&mac_size, 1, sizeof(mac_size), fp);
	ret = fread(&meta_size, 1, sizeof(meta_size), fp);
	while (meta_size--) {
		ret = fread(&byte, 1, 1, fp);
		local_meta+=byte;
	}
	fclose(fp);
	
	if (mac_size != __peer_status.total)
		return -1;
	__peer_status.log_timestamp = local_timestamp;
	meta = local_meta;
	
	fp=fopen((string() + "dsm._checkpoint/" "journal-"+to_string(__peer_status.log_timestamp)+"-part" + to_string(__peer_status.rank)).c_str(), "rb");
	if (!fp)
		return -1;
	ret = fread(&table_size, 1, sizeof(table_size), fp);
	for (int i=0;i<table_size;i++) {
		int items;
		void *key;
		ret = fread(&items, 1, sizeof(items), fp);
		tables[i].clear();
		while (items--) {
			static char buffer[MAX_MSG_LENGTH];
			ret = fread(&key, 1, sizeof(void *), fp);
			ret = fread(buffer, 1, sizeof(serializable_data), fp);
			ret = fread(buffer+sizeof(serializable_data), 1, ((serializable_data*)buffer)->size, fp);
			tables[i].setValue(key, (serializable_data*)buffer);
		}
	}
	fclose(fp);
	
	while (tables->swap && tables->inMemoryKeys() > IN_MEMORY_KEY_LIMIT) {
		tables->swapOut();
	}
	
	P2P_Master_Log("Checkpoint with timestamp <"+to_string(__peer_status.log_timestamp)+"> was loaded.");
	return table_size;
}

void P2P_Synchronize(unsigned int table_cnt, DistriSharedMap *tables, string meta) {
	for (auto it: __peer_status.table_map)
		it.second->flush();
	metadata md = {P2P_DO_MARKER, 0};
	for (int i=0;i<__peer_status.total;i++)
		MPI_Send(&md,sizeof(metadata),MPI_CHAR,i,0,MPI_COMM_WORLD);
	__peer_status.sync_event.wait();
	
	if (!table_cnt)
		return;
	
	__peer_status.log_timestamp;
	// log_timestamp
	// FILE *fp=fopen("", );
	// "dsm._checkpoint/"
	mkdir("dsm._checkpoint/", 0775);
	
	FILE *fp=fopen((string() + "dsm._checkpoint/" "journal-"+to_string(++__peer_status.log_timestamp)+"-part" + to_string(__peer_status.rank)).c_str(), "wb");
	fwrite(&table_cnt, 1, sizeof(table_cnt), fp);
	for (int i=0;i<table_cnt;i++) {
		auto it = tables[i].p2p_iterator();
		// printf("[[ %d ]]\n", it.size());
		unsigned int item_size = it.size();
		fwrite(&item_size, 1, sizeof(item_size), fp);
		while (it.next()) {
			auto data = it.data();
			fwrite(&data->first, 1, sizeof(data->first), fp);
			fwrite(data->second, 1, sizeof(serializable_data)+data->second->size, fp);
		}
	}
	fclose(fp);
			
	if (__peer_status.rank == 0) {
		fp=fopen("dsm._checkpoint/" "journal.tmp", "wb");
		fwrite(&__peer_status.log_timestamp, 1, sizeof(__peer_status.log_timestamp), fp);
		fwrite(&__peer_status.total, 1, sizeof(__peer_status.total), fp);
		unsigned int meta_size = meta.size();
		fwrite(&meta_size, 1, sizeof(meta_size), fp);
		fwrite(meta.c_str(), 1, meta_size, fp);
		fclose(fp);
	}
	
	tables->rwmtx.write_lock();
	while (tables->swap && tables->inMemoryKeys() > IN_MEMORY_KEY_LIMIT) {
		tables->swapOut();
	}
	tables->rwmtx.unlock();
	
	for (int i=0;i<__peer_status.total;i++)
		MPI_Send(&md,sizeof(metadata),MPI_CHAR,i,0,MPI_COMM_WORLD);
	__peer_status.sync_event.wait();
	
	if (__peer_status.rank == 0) {
		if (system("mv " "dsm._checkpoint/" "journal.tmp " "dsm._checkpoint/" "journal")!=0)
			P2P_Master_Log("Cannot remame 'journal.tmp' to 'journal'");
	}
	P2P_Master_Log("Checkpoint was made with timestamp <"+to_string(__peer_status.log_timestamp)+">.");
}

void P2P_Initialization(int argc, char **argv) {
	__peer_status.uuid_allocator = 0;
	__peer_status.marker_counter = 0;
	__peer_status.table_map.clear();
	
	assert(WEAK_BUFFER <= MAX_MSG_LENGTH);
	
	int provided;
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
	if (provided != MPI_THREAD_MULTIPLE) {
		P2P_Master_Log("MPI with threads not supported!");
		MPI_Abort(MPI_COMM_WORLD, 1);
		return;
	}
	MPI_Comm_size(MPI_COMM_WORLD, &__peer_status.total);
	MPI_Comm_rank(MPI_COMM_WORLD, &__peer_status.rank);
	
	assert(__peer_status.total <= MAX_WORKER_NUM);
	
	__peer_status.mail_listener = new thread(P2P_Recv_Listener);
	
	P2P_Synchronize();
}

void P2P_Finilization() {
	metadata md = {P2P_DO_SHUTDOWN};
	for (int i=0;!__peer_status.rank && i<__peer_status.total;i++)
		MPI_Send(&md,sizeof(metadata),MPI_CHAR,i,0,MPI_COMM_WORLD);
	__peer_status.mail_listener->join();
	MPI_Finalize();
}

//==================== 用于 PageRank 的分布式数据表实例：Key为顶点号，Value为权重积分

class PageRankSharedMap: public DistriSharedMap {
	
public:
	PageRankSharedMap(): DistriSharedMap(true) {
	}
	
	virtual void combineValue(serializable_data *&value, const void *incValue) {
		value->size = sizeof(float);
		
		*(float*)&(value->data) += *(float*)&incValue;
	}
};

//==================== 用于 PageRank 的分布式数据表实例：Key为顶点号，Value为邻接边的动态数组

class GraphSharedMap: public DistriSharedMap {
	
public:
	
	virtual void combineValue(serializable_data *&value, const void *incValue) {
		int out = *(int*)&incValue;
		if (value->size + sizeof(out) > value->alloc) {
			value = (serializable_data *)realloc(value, sizeof(serializable_data) + BUFFER_UNIT + value->alloc);
			value->alloc += BUFFER_UNIT;
		}
		memcpy(value->data + value->size, &out, sizeof(out));
		value->size += sizeof(out);
		//printf("=================== %d %lu\n", value->size, incValue);
	}
	
	bool p2p_load_graph(const char *path) {
		P2P_Synchronize();
		
		FILE *fp = fopen(path, "r");
		if (!fp)
			return false;
		
		fseek(fp, 0, SEEK_END);
		long length = ftell(fp);
		rewind(fp);
		
		long block_size = (length+__peer_status.total-1)/__peer_status.total;
		long left=min(length, __peer_status.rank*block_size), right=max(left, min(length, left+block_size));
		
		static char record[BUFFER_UNIT];
		if (__peer_status.rank && left > 0) {
			fseek(fp, left-1, SEEK_SET);
			assert(fgets(record, sizeof(record), fp)==record);
		}
		int tot=0;
		unsigned long u, v;
		while (ftell(fp)<right) {
			assert(fgets(record, sizeof(record), fp)==record);
			if (~sscanf(record, "%lu%lu", &u, &v)) {
				//printf("##### %lu %lu\n", (void*)u, (void*)v);
				updateValue((void*)u, (void*)v);
#ifndef DIRECTED_GRAPH
				updateValue((void*)v, (void*)u);
#endif
				tot++;
			}
			//printf("%ld %ld %d\n", ftell(fp), right, rank);
		}
		//printf("%d: %ld, %ld, %d\n", __peer_status.rank, left, right, tot);
		fclose(fp);
		
		P2P_Synchronize();
		return true;
	}
};


//==================== PageRank on DistriSharedMap 的用户接口部分

int main(int argc, char **argv) {
	
	P2P_Initialization(argc, argv);
	
	// 数据表的创建
	GraphSharedMap *graphMap = new GraphSharedMap();
	PageRankSharedMap *curr = new PageRankSharedMap(), *next = new PageRankSharedMap();
	
	timespec ts, te;
	clock_gettime(CLOCK_REALTIME, &ts);
	
	// 分布式加载图数据（每个节点负责一段的数据，非集中分配方式）
	graphMap->p2p_load_graph(GRAPH_PATH);
	
	clock_gettime(CLOCK_REALTIME, &te);
	P2P_Master_Log("Finish loading graph distributedly in "+to_string(te.tv_sec - ts.tv_sec+ (te.tv_nsec-ts.tv_nsec) * 1e-9)+" s.");
	
	ts = te;
	
	string meta = "0";
	P2P_Try_Restore(curr, meta);
	
	int step = atoi(meta.c_str()) + 1;
	//printf("[[%s]]\n", meta.c_str());
	
	if (step <= PAGERANK_TOT_STEP)
		P2P_Master_Log("Iterating step started from "+to_string(step)+".");
	
	serializable_data* init_support = create_serializable_data();
	*(float*)init_support->data = 1.0;
	
	//printf("[[%lu]]\n", graphMap->p2p_iterator().size());

	auto it = graphMap->p2p_iterator();
	
	// PageRank 主迭代过程
	for (; step<=PAGERANK_TOT_STEP; ++step) {
		auto it = graphMap->p2p_iterator();
		while (it.next()) {
			int vertex = *(int*)& it.data()->first, edge_cnt = it.data()->second->size/sizeof(int), *outs = (int*)(it.data()->second->data);
			//printf("%d %d\n", vertex, edge_cnt);
			// printf("Rank %d (%d):", vertex, edge_cnt);
			// for (int p = 0; p < edge_cnt; p++)
			// 	  printf("%d ", outs[p]);
			
			float support = *(float*)curr->getValue(it.data()->first, init_support)->data / edge_cnt;
			for (int p = 0; p < edge_cnt; p++) {
				next->updateValue((void*)(unsigned long)outs[p], (void*)(unsigned long)(unsigned int&)support);
				// printf(" to %lu as %f,", (unsigned long)outs[p], support);
			}
			// putchar('\n');
		}
		
		swap(curr, next);
		next->clear();
		
		clock_gettime(CLOCK_REALTIME, &te);
		P2P_Master_Log("Finish superstep "+to_string(step)+" in "+to_string(te.tv_sec - ts.tv_sec+ (te.tv_nsec-ts.tv_nsec) * 1e-9)+" s.");
		ts = te;
		
		// 必须要同步，否则其他Worker可能预先进入下一循环开始发送消息，而本Worker的swap()还未执行结束，导致本Worker把新循环消息更新到旧指针
		if (step % CHECKPOINT_INTERVAL == 0 || step == PAGERANK_TOT_STEP)
			P2P_Synchronize(1, curr, to_string(step));
		else
			P2P_Synchronize();
		P2P_Node_Log("Number of in-memory key-pair: "+to_string(curr->inMemoryKeys(true))+".");
	}
	
	// PageRank 结果分布式保存（分布输出 —— 每个节点保存自身一段数据）
	curr->saveAsTextFile("dsm.page_rank.out", [&](const DistriSharedMap::table_t::iterator &it) {
		int vertex = *(int*)& it->first, edge_cnt = it->second->size/sizeof(int);
		return to_string(*(int*)& it->first) + " " + to_string(*(float*) it->second->data);
	});
	
	// 输出图的顶点度（分布输出 —— 每个节点保存自身一段数据）
	graphMap->saveAsTextFile("dsm.graph_degree.out", [&](const DistriSharedMap::table_t::iterator &it) {
		int vertex = *(int*)& it->first, edge_cnt = it->second->size/sizeof(int);
		return to_string(vertex) + " " + to_string(edge_cnt);
	});
	
	clock_gettime(CLOCK_REALTIME, &te);
	P2P_Master_Log("Finish writing results in "+to_string(te.tv_sec - ts.tv_sec+ (te.tv_nsec-ts.tv_nsec) * 1e-9)+" s.");
	
	// 资源的销毁
	P2P_Finilization();
	
	delete graphMap;
	delete curr;
	delete next;
	return 0;
}

