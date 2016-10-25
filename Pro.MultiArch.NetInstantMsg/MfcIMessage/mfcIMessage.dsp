# Microsoft Developer Studio Project File - Name="mfcIMessage" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=mfcIMessage - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mfcIMessage.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mfcIMessage.mak" CFG="mfcIMessage - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mfcIMessage - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_UNICODE" /Yu"stdafx.h" /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386
# Begin Target

# Name "mfcIMessage - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\mfcIMessage.cpp
DEP_CPP_MFCIM=\
	".\Base64.h"\
	".\CommonLib.h"\
	".\mfcIMessage.h"\
	".\mfcIMessageDlg.h"\
	".\MsgChatDlg.h"\
	".\Socket.h"\
	".\StdAfx.h"\
	".\TreeList.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mfcIMessage.rc
# End Source File
# Begin Source File

SOURCE=.\mfcIMessageDlg.cpp
DEP_CPP_MFCIME=\
	".\Base64.h"\
	".\CommonLib.h"\
	".\mfcIMessage.h"\
	".\mfcIMessageDlg.h"\
	".\MsgChatDlg.h"\
	".\Socket.h"\
	".\StdAfx.h"\
	".\TreeList.h"\
	
# End Source File
# Begin Source File

SOURCE=.\MsgChatDlg.cpp
DEP_CPP_MSGCH=\
	".\Base64.h"\
	".\CommonLib.h"\
	".\mfcIMessage.h"\
	".\MsgChatDlg.h"\
	".\Socket.h"\
	".\StdAfx.h"\
	".\TreeList.h"\
	
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\Base64.h"\
	".\CommonLib.h"\
	".\MsgChatDlg.h"\
	".\Socket.h"\
	".\StdAfx.h"\
	".\TreeList.h"\
	
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TreeList.cpp
DEP_CPP_TREEL=\
	".\Base64.h"\
	".\CommonLib.h"\
	".\mfcIMessage.h"\
	".\MsgChatDlg.h"\
	".\Socket.h"\
	".\StdAfx.h"\
	".\TreeList.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Base64.h
# End Source File
# Begin Source File

SOURCE=.\CommonLib.h
# End Source File
# Begin Source File

SOURCE=.\mfcIMessage.h
# End Source File
# Begin Source File

SOURCE=.\mfcIMessageDlg.h
# End Source File
# Begin Source File

SOURCE=.\MsgChatDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Socket.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TreeList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\icoDir.ico
# End Source File
# Begin Source File

SOURCE=.\res\icoDirReq.ico
# End Source File
# Begin Source File

SOURCE=.\res\icoHead.ico
# End Source File
# Begin Source File

SOURCE=.\res\icoOffline.ico
# End Source File
# Begin Source File

SOURCE=.\res\icoOnline.ico
# End Source File
# Begin Source File

SOURCE=.\res\icoReq.ico
# End Source File
# Begin Source File

SOURCE=.\res\mfcIMessage.ico
# End Source File
# Begin Source File

SOURCE=.\res\mfcIMessage.rc2
# End Source File
# End Group
# End Target
# End Project
