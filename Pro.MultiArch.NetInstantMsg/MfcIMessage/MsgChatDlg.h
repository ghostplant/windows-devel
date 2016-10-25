#if !defined(AFX_MSGCHATDLG_H__829E5136_5A50_402E_9CC5_FEBF11C1F5D3__INCLUDED_)
#define AFX_MSGCHATDLG_H__829E5136_5A50_402E_9CC5_FEBF11C1F5D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgChatDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMsgChatDlg dialog

class CMsgChatDlg : public CDialog
{
// Construction
public:
	CString friendId;
	CMsgChatDlg(CString friendId,CTreeCtrl *treeList,CWnd* pParent = NULL);   // standard constructor

	void InsertMessage(CString name,CString text);

	void Display();
// Dialog Data
	//{{AFX_DATA(CMsgChatDlg)
	enum { IDD = IDD_CHATDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgChatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgChatDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGCHATDLG_H__829E5136_5A50_402E_9CC5_FEBF11C1F5D3__INCLUDED_)
