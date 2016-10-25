@echo off
set path=bin;..\bin
moc qtmain.cpp > qtmain.h
g++ qtmain.cpp -o qtmain.exe -Iinclude -Llib -lQtCore4 -lQtGui4 -mwindows
del qtmain.h
pause
qtmain