// mfcIMessage.h : main header file for the MFCIMESSAGE application
//

#if !defined(AFX_MFCIMESSAGE_H__314CF0F6_731C_4455_857F_AEF5C0996AC7__INCLUDED_)
#define AFX_MFCIMESSAGE_H__314CF0F6_731C_4455_857F_AEF5C0996AC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfcIMessageApp:
// See mfcIMessage.cpp for the implementation of this class
//

class CMfcIMessageApp : public CWinApp
{
public:
	CMfcIMessageApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcIMessageApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMfcIMessageApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCIMESSAGE_H__314CF0F6_731C_4455_857F_AEF5C0996AC7__INCLUDED_)
