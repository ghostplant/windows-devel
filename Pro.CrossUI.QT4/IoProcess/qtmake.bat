@echo off
set path=bin;..\bin
g++ qtmain.cpp -o qtmain.exe -Iinclude -Llib -lQtCore4
pause
qtmain
pause