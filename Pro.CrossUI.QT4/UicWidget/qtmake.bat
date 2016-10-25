@echo off
set path=bin;..\bin
uic widget.ui > ui_widget.h
moc widget.cpp > widget.h
g++ widget.cpp -o widget.exe -Iinclude -Llib -lQtCore4 -lQtGui4 -mwindows
del ui_widget.h
del widget.h
pause
widget