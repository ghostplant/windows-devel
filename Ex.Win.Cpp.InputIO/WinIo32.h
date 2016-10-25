// Reference: www.internals.com
#ifndef WIN_IO_32_H
#define WIN_IO_32_H

#include <windows.h>

typedef char (*_GetPortVal)(short wPortAddr, int* pdwPortVal, char bSize);
typedef char (*_SetPortVal)(short wPortAddr, int dwPortVal, char bSize);
typedef char (*_InitializeWinIo)();
typedef char (*_ShutdownWinIo)();

typedef struct {
  HMODULE hDllLib;
  _GetPortVal GetPortVal;
  _SetPortVal SetPortVal;
  _InitializeWinIo InitializeWinIo;
  _ShutdownWinIo ShutdownWinIo;
} winio_driver;

static void KBCWait4IBE(winio_driver *drv) {
  int dwVal;
  while (drv->GetPortVal(0x64, &dwVal, 1), dwVal&0x2);
}

////////////////////////////// Public Declared Function //////////////////////////////////

void init_winio_driver(winio_driver *drv) {
  drv->hDllLib = LoadLibrary("WinIo32.dll");
  if (!drv->hDllLib)
    exit(0);
  drv->GetPortVal=(_GetPortVal)GetProcAddress(drv->hDllLib,"GetPortVal");
  drv->SetPortVal=(_SetPortVal)GetProcAddress(drv->hDllLib,"SetPortVal");
  drv->InitializeWinIo=(_InitializeWinIo)GetProcAddress(drv->hDllLib,"InitializeWinIo");
  drv->ShutdownWinIo=(_ShutdownWinIo)GetProcAddress(drv->hDllLib,"ShutdownWinIo");
  if (!drv->InitializeWinIo())
    exit(0);
}

void free_winio_driver(winio_driver *drv) {
  drv->ShutdownWinIo();
  FreeLibrary(drv->hDllLib);
}

void MyKeyDown(winio_driver *drv, int vKey) {
  int dwVal,btScancode=MapVirtualKey(vKey, 0);
  KBCWait4IBE(drv);
  drv->SetPortVal(0x64, 0xd2, 1);
  KBCWait4IBE(drv);
  drv->SetPortVal(0x60, btScancode, 1);
}

void MyKeyUp(winio_driver *drv, int vKey) {
  int btScancode=MapVirtualKey(vKey, 0);
  KBCWait4IBE(drv);
  drv->SetPortVal(0x64, 0xd2, 1);
  KBCWait4IBE(drv);
  drv->SetPortVal(0x60, btScancode|0x80, 1);
}
#endif
