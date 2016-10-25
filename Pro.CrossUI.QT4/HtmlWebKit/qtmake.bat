@echo off
set path=bin;..\bin
g++ qtmain.cpp -o qtmain.exe -Iinclude -Llib -lQtCore4 -lQtGui4 -lQtWebKit4 -mwindows
pause
qtmain