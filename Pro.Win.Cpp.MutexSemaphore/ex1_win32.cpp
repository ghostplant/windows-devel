#include <stdio.h>
#include <windows.h>

#define MAX_LEN 1024

int main() {
	static char path[MAX_LEN],arg[MAX_LEN]="";
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	
	freopen(".\\batch","r",stdin);
	while (gets(path)) {
				// ·��  ���� ����  �߰�  �̸�  ��ʹ�ø����̿���̨ ���ȼ� ����  ���� ����
		if (!CreateProcess(path, arg, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
			fprintf(stderr,"��������ʧ�ܣ�����ţ�%d\n", GetLastError());
			ExitProcess(1);
		}
		fprintf(stdout,"�ɹ��������̣�%s (���̺ţ�%d)\n",path,pi.dwProcessId);
		Sleep(500);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	return 0;
}
