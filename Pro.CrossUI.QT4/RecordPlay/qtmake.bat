@echo off
set path=bin;..\bin
moc main.cpp > main.h
g++ main.cpp -o main.exe -Iinclude -Llib -lQtCore4 -lQtMultimedia4
del main.h
pause
main
pause