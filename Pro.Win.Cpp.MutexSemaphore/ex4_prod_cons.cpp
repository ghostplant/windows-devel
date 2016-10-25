#include <cstdio>
#include <vector>
#include <cmath>
#include <windows.h>

using namespace std;

const int BUFFER_SIZE=9;
const int conDelay=1900, proDelay=300, delay=500;

struct Task {
		int threadId, arg[2];
		char role[2];
} per;

class Semaphore {
private:
	HANDLE hSemaphore;
public:
	Semaphore(int n,int init) {
		hSemaphore=CreateSemaphore(NULL,init,n,NULL);
	}

	bool P(int timeout = -1) {
		return WaitForSingleObject(hSemaphore,timeout)==WAIT_OBJECT_0;
	}

	void V() {
		ReleaseSemaphore(hSemaphore,1,NULL);
	}

	~Semaphore() {
		CloseHandle(hSemaphore);
	}
} empty(BUFFER_SIZE,BUFFER_SIZE),full(BUFFER_SIZE,0),mutex(1,1);

int firstPrimeAfter(int n) {
	if (n<=2)  return (n<2)?2:3;
	if (!(n&1))  --n;
	while (true) {
		n+=2;
		int i,bound=(int)sqrt((double)n);
		for (i=3;i<=bound;i+=2)
			if (n%i==0)
				break;
		if (i>bound)
			return n;
	}
}

int buffer[BUFFER_SIZE], pit=0, cit=0, nprod, ncons; // 临界资源

void push_back(Task *task,int prime) {
	Sleep(230);
	buffer[pit]=task->arg[0]=prime;
	pit=(pit+1)%BUFFER_SIZE;
	printf("生产者线程%d：向缓冲区添加了一个素数%d。\n",task->threadId,task->arg[0]);
}

void pop_front(Task* task) {
	Sleep(150);
	int popul=buffer[cit];
	cit=(cit+1)%BUFFER_SIZE;
	printf("消费者线程%d：从缓冲区取出了一个素数%d。\n",task->threadId,popul);
}

void producing(Task* task) {
	int prime;
	while (prime=firstPrimeAfter(task->arg[0]), prime<=task->arg[1]) { // 产生一个范围内的素数
		while (!empty.P(delay)) {
			mutex.P();
			bool noConsumer=!ncons;
			mutex.V();
			if (noConsumer) { // 说明缓冲区满，并且无消费者
				printf("生产者线程%d：【缓冲区满且无消费者，已完成自己的工作】\n",task->threadId);
				return;
			}
		}
		mutex.P();
		push_back(task,prime);
		mutex.V();
		full.V();
	}
	mutex.P();
	--nprod;
	mutex.V();
	printf("生产者线程%d：【生产结束，已完成自己的工作】\n",task->threadId);
}

void consuming(Task* task) {
	while (task->arg[0]--) {
		while (!full.P(delay)) {
			mutex.P();
			bool noProducer=!nprod;
			mutex.V();
			if (noProducer) { // 说明缓冲区空，并且无生产者
				printf("消费者线程%d：【缓冲区空且无生产者，已完成自己的工作】\n",task->threadId);
				return;
			}
		}
		mutex.P();
		pop_front(task);
		mutex.V();
		empty.V();
	}
	mutex.P();
	--ncons;
	mutex.V();
	printf("消费者线程%d：【消费结束，已完成自己的工作】\n",task->threadId);
}

int main() {
	freopen("ex4.dat","r",stdin);
	vector<Task> taskList;
	nprod=ncons=0;
	while (~scanf("%d%s%d%d",&per.threadId,per.role,per.arg,per.arg+1)) {
		switch (*per.role) {
			case 'W': case 'w':
				--per.arg[0], ++nprod; break;
			case 'D': case 'd':
				ncons++; break;
		}
		taskList.push_back(per);
	}

	int count=taskList.size();
	HANDLE *hThread=new HANDLE[count];
	for (int i=0;i<count;i++) {
		switch (*taskList[i].role) {
			case 'W': case 'w':
				hThread[i]=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(producing), &taskList[i], 0, NULL);
				break;
			case 'D': case 'd':
				hThread[i]=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(consuming), &taskList[i], 0, NULL);
				break;
		}
	}
	WaitForMultipleObjects(count,hThread,TRUE, -1);
	printf("主函数：所有线程已经执行完毕！\n");
	delete []hThread;
	return 0;
}