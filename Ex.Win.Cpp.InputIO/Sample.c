#include "WinIo32.h"

winio_driver drv;

int main() {
  init_winio_driver(&drv);
  while (1) {
    MyKeyDown(&drv,'Z');
    Sleep(100);
    MyKeyUp(&drv,'Z');
    Sleep(1000);
  }
  free_winio_driver(&drv);
  return 0;
}