#include <stdio.h>
#include <windows.h>

#define MAX_LEN 1024

int main() {
	static char path[MAX_LEN],arg[MAX_LEN]="";
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	
	freopen(".\\batch","r",stdin);
	while (gets(path)) {
				// 路径  参数 进安  线安  继父  不使用父进程控制台 优先级 环境  启属 进属
		if (!CreateProcess(path, arg, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
			fprintf(stderr,"创建进程失败，错误号：%d\n", GetLastError());
			ExitProcess(1);
		}
		fprintf(stdout,"成功创建进程：%s (进程号：%d)\n",path,pi.dwProcessId);
		Sleep(500);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	return 0;
}
