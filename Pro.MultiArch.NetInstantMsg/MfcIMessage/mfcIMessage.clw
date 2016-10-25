; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTreeList
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mfcIMessage.h"

ClassCount=5
Class1=CMfcIMessageApp
Class2=CMfcIMessageDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_LOGIN
Resource2=IDR_MAINFRAME
Resource3=IDD_MAINFRAME
Class4=CTreeList
Resource4=IDD_CHATDLG
Class5=CMsgChatDlg
Resource5=IDD_ABOUTBOX
Resource6=IDR_POPUP

[CLS:CMfcIMessageApp]
Type=0
HeaderFile=mfcIMessage.h
ImplementationFile=mfcIMessage.cpp
Filter=N
LastObject=CMfcIMessageApp

[CLS:CMfcIMessageDlg]
Type=0
HeaderFile=mfcIMessageDlg.h
ImplementationFile=mfcIMessageDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMfcIMessageDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=mfcIMessageDlg.h
ImplementationFile=mfcIMessageDlg.cpp
Filter=D
LastObject=CAboutDlg
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LOGIN]
Type=1
Class=CMfcIMessageDlg
ControlCount=5
Control1=IDC_USERNAME,edit,1350631552
Control2=IDC_STATIC,static,1342308352
Control3=IDC_LOGIN,button,1342242817
Control4=IDC_PASSWORD,edit,1350631584
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_MAINFRAME]
Type=1
Class=CMfcIMessageDlg
ControlCount=5
Control1=IDC_LB1,static,1342308352
Control2=IDC_LB2,static,1342308352
Control3=IDC_USERNAME,edit,1350631552
Control4=IDC_PASSWORD,edit,1350631584
Control5=IDC_LOGIN,button,1342242817

[CLS:CTreeList]
Type=0
HeaderFile=TreeList.h
ImplementationFile=TreeList.cpp
BaseClass=CTreeCtrl
Filter=W
LastObject=CTreeList
VirtualFilter=GWC

[DLG:IDD_CHATDLG]
Type=1
Class=CMsgChatDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHATTEXT,edit,1350637700
Control4=IDC_SENDTEXT,edit,1350635652
Control5=IDC_STATIC,static,1342308352

[CLS:CMsgChatDlg]
Type=0
HeaderFile=MsgChatDlg.h
ImplementationFile=MsgChatDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMsgChatDlg

[MNU:IDR_POPUP]
Type=1
Class=?
Command1=ID_MENU_NEW
CommandCount=1

