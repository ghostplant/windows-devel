// MsgChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfcIMessage.h"
#include "MsgChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgChatDlg dialog


CMsgChatDlg::CMsgChatDlg(CString friendId,CTreeCtrl *treeList,CWnd* pParent /*=NULL*/)
	: CDialog(CMsgChatDlg::IDD, pParent)
{
	this->friendId=friendId;
	treeMain=(CTreeList*)treeList;
	m_socket=treeMain->socket;
	//{{AFX_DATA_INIT(CMsgChatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMsgChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgChatDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgChatDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgChatDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgChatDlg message handlers
void CMsgChatDlg::InsertMessage(CString name,CString text) {
	CString chatText;
	GetDlgItemText(IDC_CHATTEXT,chatText);
	chatText+=L"来自 "+name+L" 的会话：\r\n"+text+"\r\n\r\n";
	SetDlgItemText(IDC_CHATTEXT,chatText);
}

void CMsgChatDlg::Display() {
	treeMain->getChatByFid[friendId] = this;

	Create(IDD_CHATDLG);

	SetWindowText(L"与 "+treeMain->GetItemText(treeMain->getItemByFid[friendId]) + L" 的对话中");

	ShowWindow(SW_SHOW);
	UpdateWindow();

	RECT rect;
	this->GetWindowRect(&rect);
	int x=(rect.left+rect.right)>>1,y=(rect.bottom+rect.top+200)>>1;
	SetCursorPos(x,y);
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}

void CMsgChatDlg::OnOK() 
{
	CString sendText;
	GetDlgItemText(IDC_SENDTEXT,sendText);
	SetDlgItemText(IDC_SENDTEXT,L"");
	CString self;
	AfxGetApp()->m_pMainWnd->GetWindowText(self);
	InsertMessage(self.Right(self.GetLength()-5),sendText);
	CommonLib::Command("sendmsg").addArg(friendId).addArg(sendText).execute();
}

void CMsgChatDlg::OnCancel() 
{
	treeMain->getChatByFid.erase(friendId);	
	CDialog::OnCancel();
}
