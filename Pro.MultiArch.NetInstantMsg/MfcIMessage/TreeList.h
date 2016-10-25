#if !defined(AFX_TREELIST_H__1CD55056_8415_472A_B4B0_E3BA320F4716__INCLUDED_)
#define AFX_TREELIST_H__1CD55056_8415_472A_B4B0_E3BA320F4716__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeList.h : header file
//

#include <map>
#include <vector>
#include "MsgChatDlg.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CTreeList window

class CTreeList : public CTreeCtrl
{
// Construction
public:
	CTreeList(const Socket &socket);

// Attributes
public:

// Operations
public:
	HTREEITEM unReadedMsg;
	Socket socket;
	vector<CString> getFidByData;
	map<DWORD,HTREEITEM> getItemByGid;
	map<CString,int> getGidByFid;
	map<CString,HTREEITEM> getItemByFid;
	map<CString,CMsgChatDlg*> getChatByFid;
	multimap<CString,CString> getUnmsgsByFid;

	void ExpandTree(HTREEITEM hItem) {
		Expand(hItem, TVE_EXPAND);
		Select(hItem,TVGN_FIRSTVISIBLE);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeList)
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual ~CTreeList();

	// Generated message map functions
protected:

	//{{AFX_MSG(CTreeList)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREELIST_H__1CD55056_8415_472A_B4B0_E3BA320F4716__INCLUDED_)
