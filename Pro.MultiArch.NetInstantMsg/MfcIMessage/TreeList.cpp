// TreeList.cpp : implementation file
//

#include "stdafx.h"
#include "mfcIMessage.h"
#include "TreeList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeList

CTreeList::CTreeList(const Socket &socket)
{
	m_socket=socket;
	this->socket=socket;
}

CTreeList::~CTreeList()
{
}


BEGIN_MESSAGE_MAP(CTreeList, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeList)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeList message handlers

void CTreeList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	HTREEITEM item=CTreeCtrl::HitTest(point);
	if (item) {
		if (item==this->unReadedMsg) {
			if (getUnMsg(this)) {
				multimap<CString,CString>::iterator it = getUnmsgsByFid.begin(), end = getUnmsgsByFid.upper_bound(it->first);
				CString fid=it->first;
				while (it!=end) {
					CString msg=it->second;
					map<CString,CMsgChatDlg*>::iterator chatIt=getChatByFid.find(fid);
					CMsgChatDlg *chatDlg;
					if (chatIt==getChatByFid.end()) {
						chatDlg=new CMsgChatDlg(fid, this);
						chatDlg->Display();
					} else {
						chatDlg=chatIt->second;
						chatDlg->SetFocus();
					}
					chatDlg->InsertMessage(fid,msg);
					++it;
				}
				getUnmsgsByFid.erase(fid);
				setUnMsg(this,getUnmsgsByFid.size());
			} else
				MessageBox(L"目前没有未读消息！");
		} else {
			int id=GetItemData(item);
			if (id>=0) return;
			id=-id-1;
			CString fid=getFidByData[id];
			map<CString,CMsgChatDlg*>::iterator it=getChatByFid.find(fid);
			CMsgChatDlg *chatDlg;
			if (it==getChatByFid.end()) {
				chatDlg=new CMsgChatDlg(fid, this);
				chatDlg->Display();
			} else {
				chatDlg=it->second;
				chatDlg->SetFocus();
			}
		}
	}
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

void CTreeList::OnRButtonDown(UINT nFlags, CPoint point) 
{
	HTREEITEM item=CTreeCtrl::HitTest(point);
	if (item) {
		if (item==this->unReadedMsg) {
			CMenu menu;
			menu.LoadMenu(IDR_POPUP);
			CPoint pt;
			GetCursorPos(&pt);
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
		} else {
		}
	}
	CTreeCtrl::OnRButtonDown(nFlags, point);
}
