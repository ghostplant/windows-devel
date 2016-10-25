#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
	HINSTANCE hDll = LoadLibrary("dynamic.dll");
	typedef int(*lpAddFun)(int, int);
	lpAddFun pointer=(lpAddFun)GetProcAddress(hDll, "myadd");
	char ans[]={pointer(1,2)+'0',0};
	MessageBox(0,ans,0,0);
	return 0;
}
