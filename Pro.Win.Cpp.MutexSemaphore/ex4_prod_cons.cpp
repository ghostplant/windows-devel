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

int buffer[BUFFER_SIZE], pit=0, cit=0, nprod, ncons; // �ٽ���Դ

void push_back(Task *task,int prime) {
	Sleep(230);
	buffer[pit]=task->arg[0]=prime;
	pit=(pit+1)%BUFFER_SIZE;
	printf("�������߳�%d���򻺳��������һ������%d��\n",task->threadId,task->arg[0]);
}

void pop_front(Task* task) {
	Sleep(150);
	int popul=buffer[cit];
	cit=(cit+1)%BUFFER_SIZE;
	printf("�������߳�%d���ӻ�����ȡ����һ������%d��\n",task->threadId,popul);
}

void producing(Task* task) {
	int prime;
	while (prime=firstPrimeAfter(task->arg[0]), prime<=task->arg[1]) { // ����һ����Χ�ڵ�����
		while (!empty.P(delay)) {
			mutex.P();
			bool noConsumer=!ncons;
			mutex.V();
			if (noConsumer) { // ˵������������������������
				printf("�������߳�%d���������������������ߣ�������Լ��Ĺ�����\n",task->threadId);
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
	printf("�������߳�%d��������������������Լ��Ĺ�����\n",task->threadId);
}

void consuming(Task* task) {
	while (task->arg[0]--) {
		while (!full.P(delay)) {
			mutex.P();
			bool noProducer=!nprod;
			mutex.V();
			if (noProducer) { // ˵���������գ�������������
				printf("�������߳�%d���������������������ߣ�������Լ��Ĺ�����\n",task->threadId);
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
	printf("�������߳�%d�������ѽ�����������Լ��Ĺ�����\n",task->threadId);
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
	printf("�������������߳��Ѿ�ִ����ϣ�\n");
	delete []hThread;
	return 0;
}