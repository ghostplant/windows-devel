// mfcIMessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfcIMessage.h"
#include "mfcIMessageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcIMessageDlg dialog

CMfcIMessageDlg::CMfcIMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMfcIMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMfcIMessageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMfcIMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMfcIMessageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMfcIMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CMfcIMessageDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOGIN, OnLogin)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcIMessageDlg message handlers

BOOL CMfcIMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMfcIMessageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMfcIMessageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CMfcIMessageDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMfcIMessageDlg::OnLogin()
{
	if (okButtonIgnore)
		return;
	CString username,password;
	GetDlgItemText(IDC_USERNAME,username);
	GetDlgItemText(IDC_PASSWORD,password);
	if (!username.GetLength() || !password.GetLength())
		MessageBox(L"ÇëÄú½«ÕËºÅÓëÃÜÂëÌîÐ´ÍêÕû£¡");
	else
		CommonLib::Command("log").addArg(username).addArg(password).execute();
}

void CMfcIMessageDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CWnd *ctrl;
	ctrl=(CWnd*)GetDlgItem(IDC_USERNAME);
	if (ctrl!=NULL)
		ctrl->MoveWindow(5,99,cx-11,26);
	ctrl=(CWnd*)GetDlgItem(IDC_PASSWORD);
	if (ctrl!=NULL)
		ctrl->MoveWindow(5,167,cx-11,26);
	ctrl=(CWnd*)GetDlgItem(IDC_LOGIN);
	if (ctrl!=NULL)
		ctrl->MoveWindow(5,237,cx-11,33);
	ctrl=(CWnd*)GetDlgItem(IDC_TREECTRL);
	if (ctrl!=NULL)
		ctrl->MoveWindow(0,0,cx,cy);
}

int CMfcIMessageDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ClientSocket client("202.119.81.70",4502);
	CommonLib::startConnection(client.connect());
	CWnd *win=AfxGetApp()->m_pMainWnd;
	treeMain=new CTreeList(m_socket);
	treeMain->Create(TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_NONEVENHEIGHT | WS_CHILD,CRect(0,0,151,332),win,IDC_TREECTRL);

	CImageList *images=new CImageList();
	images->Create(20,20,ILC_COLOR24,2,1);
	images->Add(AfxGetApp()->LoadIcon(IDI_DIR));
	images->Add(AfxGetApp()->LoadIcon(IDI_HEAD));
	images->Add(AfxGetApp()->LoadIcon(IDI_OFF));
	treeMain->SetImageList(images,TVSIL_NORMAL);
	return 0;
}
