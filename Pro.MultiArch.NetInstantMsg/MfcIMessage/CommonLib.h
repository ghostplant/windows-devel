#include "resource.h"
#include "TreeList.h"

static Socket m_socket;
static CTreeList *treeMain;

class SocketBuffer {
	static void write(CString bytes) {
		int len=bytes.GetLength();
		char *data=new char[len];
		for (int i=0;i<len;i++)
			data[i]=(char)bytes[i];
		m_socket.puts(data,len);
		delete []data;
	}

public:
	CString s_buffer;

	SocketBuffer& addArg(CString args) {
		s_buffer+=TCHAR('|');
		s_buffer+=Base64::Encode(args);
		return *this;
	}

	void execute() {
		s_buffer+=TCHAR(';');
		SocketBuffer::write(s_buffer);
	}
};

static DWORD WINAPI ThreadProc(LPVOID lpArgs);
static CString restBuf;

class CommonLib {

public:
	static void startConnection(Socket s) {
		m_socket=s;
		CreateThread(0,0,ThreadProc,0,0,0);
	}

	static CString read() {
		const int size=1<<13;
		static char buf[size];
		int len=m_socket.gets(buf,size);
		if (len==-1)
			return "~";
		buf[len]=0;
		return buf;
	}

	static SocketBuffer Command(CString cmd) {
		SocketBuffer sb;
		sb.s_buffer=Base64::Encode(cmd);
		return sb;
	}
};

static void TransMessage(CString key,int argc,CString *argv);

static DWORD WINAPI ThreadProc(LPVOID lpArgs) {
	CString buf;
	while ((buf=CommonLib::read())!="~") {
		int args=0;
		for (int i=0;i<buf.GetLength();i++) {
			if (buf[i]==';') {
				int cnt=0;
				CString *argv=new CString[args+1];
				for (int i=0,j=0;j<=restBuf.GetLength();j++)
					if (j==restBuf.GetLength() || restBuf[j]=='|')
						argv[cnt++]=Base64::Decode(restBuf.Mid(i,j-i)),i=j+1;
				TransMessage(*argv,args,argv+1);
				args=0, restBuf="";
				delete []argv;
			} else {
				args+=(buf[i]=='|');
				restBuf+=buf[i];
			}
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
static void setUnMsg(CTreeList *treeMain, UINT value) {
	CString uintValue;
	uintValue.Format(L"%u",value);
	treeMain->SetItemText(treeMain->unReadedMsg,"您有 "+uintValue+" 条未读消息");
}

static UINT getUnMsg(CTreeList *treeMain) {
	CString str=treeMain->GetItemText(treeMain->unReadedMsg);
	UINT sum=0;
	for (int i=3;str[i]!=' ';i++)
		sum=(sum<<3)+(sum<<1)+str[i]-'0';
	return sum;
}

static bool okButtonIgnore=false;

static void TransMessage(CString key,int argc,CString *argv) {
	if (key=="con") {
		if (argv[0]=="Y") {
			treeMain->DeleteAllItems();
			treeMain->unReadedMsg=treeMain->InsertItem(L"",1,1);
			setUnMsg(treeMain,0);
			CWnd *win=AfxGetApp()->m_pMainWnd;
			((CWnd*)win->GetDlgItem(IDC_USERNAME))->ShowWindow(SW_HIDE);
			((CWnd*)win->GetDlgItem(IDC_PASSWORD))->ShowWindow(SW_HIDE);
			((CWnd*)win->GetDlgItem(IDC_LOGIN))->ShowWindow(SW_HIDE);
			((CWnd*)win->GetDlgItem(IDC_LB1))->ShowWindow(SW_HIDE);
			((CWnd*)win->GetDlgItem(IDC_LB2))->ShowWindow(SW_HIDE);
			treeMain->ShowWindow(SW_SHOW);
			okButtonIgnore=true;
		} else
			MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd, L"密码错误，登录没有成功！", L"消息",MB_ICONINFORMATION);
	} else if (key=="nick") {
		AfxGetApp()->m_pMainWnd->SetWindowText(L"账号："+argv[0]);
	} else if (key=="crtgroup") {
		HTREEITEM item=treeMain->InsertItem(argv[1],0,0);
		DWORD id=_ttoi(argv[0]);
		treeMain->getItemByGid[id]=item;
		treeMain->SetItemData(item,id);
	} else if (key=="crtfrd") {
		int gid=_ttoi(argv[1]);
		HTREEITEM root=treeMain->getItemByGid[gid];
		HTREEITEM item=treeMain->InsertItem(argv[2],2,2,root);
		treeMain->getGidByFid[argv[0]]=gid;
		treeMain->getItemByFid[argv[0]]=item;
		treeMain->SetItemData(item,-(int)treeMain->getFidByData.size()-1);
		treeMain->getFidByData.push_back(argv[0]);
		treeMain->ExpandTree(item);
	} else if (key=="frdstt") {
		int no=(argv[1]=="True")?1:2;
		treeMain->SetItemImage(treeMain->getItemByFid[argv[0]],no,no);
	} else if (key=="recvmsg") {
		map<CString,CMsgChatDlg*>::iterator it=treeMain->getChatByFid.find(argv[0]);
		CMsgChatDlg* chatDlg;
		if (it==treeMain->getChatByFid.end()) {
			treeMain->getUnmsgsByFid.insert(make_pair<CString,CString>(argv[0],argv[1]));
			setUnMsg(treeMain,treeMain->getUnmsgsByFid.size());
		} else {
			chatDlg=it->second;
			HTREEITEM item = treeMain->getItemByFid[argv[0]];
			CString fname = treeMain->GetItemText(item);
			chatDlg->InsertMessage(fname,argv[1]);
		}
	} else if (key=="gender") {
		CString gender=(argv[0]=="True")?" ♂":" ♀",title;
		AfxGetApp()->GetMainWnd()->GetWindowText(title);
		TCHAR end=title[title.GetLength()-1];
		if (end=='♂' || end=='♀')
			title=title.Left(title.GetLength()-2);
		AfxGetApp()->GetMainWnd()->SetWindowText(title+gender);
	} else if (key=="off") {
		m_socket.close();
		MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd,L"对不起，当前账号正在被其他地点登录，您被迫下线！",L"消息",MB_ICONINFORMATION);
		exit(0);
	} else if (key=="error")
		MessageBox(0,argv[0],L"消息",MB_ICONINFORMATION);
	else
		MessageBox(0,key,0,0);
}
