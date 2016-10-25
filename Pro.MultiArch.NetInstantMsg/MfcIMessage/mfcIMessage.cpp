// mfcIMessage.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "mfcIMessage.h"
#include "mfcIMessageDlg.h"

#include "MsgChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcIMessageApp

BEGIN_MESSAGE_MAP(CMfcIMessageApp, CWinApp)
	//{{AFX_MSG_MAP(CMfcIMessageApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcIMessageApp construction

CMfcIMessageApp::CMfcIMessageApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMfcIMessageApp object

CMfcIMessageApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMfcIMessageApp initialization

BOOL CMfcIMessageApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

	CMfcIMessageDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
