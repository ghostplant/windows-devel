#include <windows.h>
#include <conio.h>
#include <fstream>
#include <stdio.h>

using namespace std;

#define INTE_PER_SEC  100
#define MAX_THREAD_NUM  64
#define MAX_FILE_NUM  32
#define MAX_STR_LEN  32

struct ThreadInfo {
	int	serial;
	char	entity;
	double	delay, persist;
};

class Semaphore {
private:
	HANDLE hSemaphore1,hSemaphore2;
	int resource;
public:
	Semaphore(int n = 1) {
		resource=n;
		hSemaphore1=CreateSemaphore(NULL,1,1,NULL);
		hSemaphore2=CreateSemaphore(NULL,0,1,NULL);
	}

	void P() {
		WaitForSingleObject(hSemaphore1,-1);
		if (--resource<0) {
			ReleaseSemaphore(hSemaphore1,1,NULL);
			WaitForSingleObject(hSemaphore2,-1);
		}
		ReleaseSemaphore(hSemaphore1,1,NULL);
	}

	void V() {
		WaitForSingleObject(hSemaphore1,-1);
		if (++resource<=0)
			ReleaseSemaphore(hSemaphore2,1,NULL);
		else
			ReleaseSemaphore(hSemaphore1,1,NULL);
	}

	~Semaphore() {
		CloseHandle(hSemaphore1);
		CloseHandle(hSemaphore2);
	}
} onReadCnt,onWriteCnt, onWriting, hasWait;

int rcount,wcount;

void LoadWorkSequence(ThreadInfo* thread_info,DWORD &n_thread) {
	ifstream inFile("sm5.dat");
	n_thread = 0;
	wcount = rcount = 0;
	while ( inFile ) {
		inFile>>thread_info[n_thread].serial;
		inFile>>thread_info[n_thread].entity;
		inFile>>thread_info[n_thread].delay;
		inFile>>thread_info[n_thread++].persist;
		inFile.get();
	}
}

namespace ReaderPriority {

	void ReaderThread(void* p) {
		int m_delay = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);
		int m_persist = (DWORD) (((ThreadInfo*)(p)) -> persist*INTE_PER_SEC);
		int m_serial = ((ThreadInfo*) (p)) -> serial;
		Sleep(m_delay);
		printf("Reader thread %d sents the reading require .\n",m_serial);

		onReadCnt.P();
		if (++rcount==1)
			onWriting.P();
		onReadCnt.V();

		printf("Reader thread %d begins to read file.\n",m_serial);
		Sleep(m_persist);
		printf("Reader thread %d finished reading file.\n",m_serial);

		onReadCnt.P();
		if (--rcount==0)
			onWriting.V();
		onReadCnt.V();
	}

	void WriterThread(void* p) {
		int m_delay = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);
		int m_persist = (DWORD) (((ThreadInfo*)(p)) -> persist*INTE_PER_SEC);
		int m_serial = ((ThreadInfo*) (p)) -> serial;
		Sleep(m_delay);
		printf("Writer thread %d sents the writing require .\n",m_serial);
	
		onWriting.P();
		printf("Writer thread %d begins to write to the file.\n",m_serial);
		Sleep(m_persist);
		printf("Writer thread %d finished Writing to the file.\n",m_serial);
		onWriting.V();
	}

	void Execute() {
		DWORD n_thread, thread_ID;
		HANDLE h_Thread[MAX_THREAD_NUM];
		ThreadInfo  thread_info[MAX_THREAD_NUM];
		LoadWorkSequence(thread_info,n_thread);

		printf( "Reader Priority:\n\n" );
		for( int i = 0; i < (int)(n_thread); i++) {
			if(thread_info[i].entity == 'R' || thread_info[1].entity == 'r')
				h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(ReaderThread), &thread_info[i], 0, &thread_ID);
			else
				h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(WriterThread), &thread_info[i], 0, &thread_ID);
		}
		WaitForMultipleObjects(n_thread,h_Thread,TRUE, -1);
		printf("All reader and writer thread have finished Operating.\n");
	}
}

namespace WriterPriority {

	void ReaderThread(void* p) {
		int m_delay = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);
		int m_persist = (DWORD) (((ThreadInfo*)(p)) -> persist*INTE_PER_SEC);
		int m_serial = ((ThreadInfo*) (p)) -> serial;
		Sleep(m_delay);
		printf("Reader thread %d sents the reading require .\n",m_serial);

		hasWait.P();
		onReadCnt.P();
		if (++rcount==1)
			onWriting.P();
		onReadCnt.V();
		hasWait.V();

		printf("Reader thread %d begins to read file.\n",m_serial);
		Sleep(m_persist);
		printf("Reader thread %d finished reading file.\n",m_serial);

		onReadCnt.P();
		if (--rcount==0)
			onWriting.V();
		onReadCnt.V();
	}

	void WriterThread(void* p) {
		int m_delay = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);
		int m_persist = (DWORD) (((ThreadInfo*)(p)) -> persist*INTE_PER_SEC);
		int m_serial = ((ThreadInfo*) (p)) -> serial;
		Sleep(m_delay);
		printf("Writer thread %d sents the writing require .\n",m_serial);
	
		onWriteCnt.P();
		if (++wcount==1)
			hasWait.P();
		onWriteCnt.V();

		onWriting.P();
		printf("Writer thread %d begins to write to the file.\n",m_serial);
		Sleep(m_persist);
		printf("Writer thread %d finished Writing to the file.\n",m_serial);
		onWriting.V();

		onWriteCnt.P();
		if (--wcount==0)
			hasWait.V();
		onWriteCnt.V();
	}
	
	void Execute() {
		DWORD n_thread, thread_ID;
		HANDLE h_Thread[MAX_THREAD_NUM];
		ThreadInfo  thread_info[MAX_THREAD_NUM];
		LoadWorkSequence(thread_info,n_thread);

		printf( "Writer Priority:\n\n" );
		for( int i = 0; i < (int)(n_thread); i++) {
			if(thread_info[i].entity == 'R' || thread_info[1].entity == 'r')
				h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(ReaderThread), &thread_info[i], 0, &thread_ID);
			else
				h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(WriterThread), &thread_info[i], 0, &thread_ID);
		}
		WaitForMultipleObjects(n_thread,h_Thread,TRUE, -1);
		printf("All reader and writer thread have finished Operating.\n");
	}
}

namespace FairPriority {

	void ReaderThread(void* p) {
		int m_delay = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);
		int m_persist = (DWORD) (((ThreadInfo*)(p)) -> persist*INTE_PER_SEC);
		int m_serial = ((ThreadInfo*) (p)) -> serial;
		Sleep(m_delay);
		printf("Reader thread %d sents the reading require .\n",m_serial);

		hasWait.P();
		onReadCnt.P();
		if (++rcount==1)
			onWriting.P();
		onReadCnt.V();
		hasWait.V();

		printf("Reader thread %d begins to read file.\n",m_serial);
		Sleep(m_persist);
		printf("Reader thread %d finished reading file.\n",m_serial);

		onReadCnt.P();
		if (--rcount==0)
			onWriting.V();
		onReadCnt.V();
	}

	void WriterThread(void* p) {
		int m_delay = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);
		int m_persist = (DWORD) (((ThreadInfo*)(p)) -> persist*INTE_PER_SEC);
		int m_serial = ((ThreadInfo*) (p)) -> serial;
		Sleep(m_delay);
		printf("Writer thread %d sents the writing require .\n",m_serial);
	
		hasWait.P();
		onWriting.P();
		printf("Writer thread %d begins to write to the file.\n",m_serial);
		Sleep(m_persist);
		printf("Writer thread %d finished Writing to the file.\n",m_serial);
		onWriting.V();
		hasWait.V();
	}
	
	void Execute() {
		DWORD n_thread, thread_ID;
		HANDLE h_Thread[MAX_THREAD_NUM];
		ThreadInfo  thread_info[MAX_THREAD_NUM];
		LoadWorkSequence(thread_info,n_thread);

		printf( "Fair Priority:\n\n" );
		for( int i = 0; i < (int)(n_thread); i++) {
			if(thread_info[i].entity == 'R' || thread_info[1].entity == 'r')
				h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(ReaderThread), &thread_info[i], 0, &thread_ID);
			else
				h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(WriterThread), &thread_info[i], 0, &thread_ID);
		}
		WaitForMultipleObjects(n_thread,h_Thread,TRUE, -1);
		printf("All reader and writer thread have finished Operating.\n");
	}
}

int main( int agrc, char* argv[] ) {
	char ch=0;
	while (ch!='4') {
		system( "cls" );
		printf("*********************************************\n"
			"       1.测试读者优先的“读者写者问题”\n"
			"       2.测试写着优先的“读者写者问题”\n"
			"       3.测试公平竞争的“读者写者问题”\n"
			"       4.退出程序\n"
			"*********************************************\n"
			"选择执行的功能(1-4): ");

		while (ch=_getch(), ch < '1' || ch > '4');
		system ( "cls" );

		switch (ch) {
			case '1':
				ReaderPriority::Execute(); break;
			case '2':
				WriterPriority::Execute(); break;
			case '3':
				FairPriority::Execute(); break;
			case '4':
				printf("感谢测试本程序！\n"); return 0;
		}
		system("pause");
	}
}
