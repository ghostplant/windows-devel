// mfcIMessageDlg.h : header file
//
#if !defined(AFX_MFCIMESSAGEDLG_H__55A6FE00_876E_4824_9D01_301639FE5034__INCLUDED_)
#define AFX_MFCIMESSAGEDLG_H__55A6FE00_876E_4824_9D01_301639FE5034__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMfcIMessageDlg dialog

class CMfcIMessageDlg : public CDialog
{
// Construction
public:
	CMfcIMessageDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMfcIMessageDlg)
	enum { IDD = IDD_MAINFRAME };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcIMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMfcIMessageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLogin();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCIMESSAGEDLG_H__55A6FE00_876E_4824_9D01_301639FE5034__INCLUDED_)
