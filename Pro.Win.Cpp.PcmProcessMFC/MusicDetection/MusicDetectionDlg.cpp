// MusicDetectionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicDetection.h"
#include "MusicDetectionDlg.h"

#include "pcmwave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMusicDetectionDlg 对话框




CMusicDetectionDlg::CMusicDetectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMusicDetectionDlg::IDD, pParent)
{
	GetCurrentDirectory(MAX_PATH,mainPath);
	*filename=0;
	g_hWndMCIWnd = MCIWndCreate(this->m_hWnd,::AfxGetApp()->m_hInstance,MCIWNDF_SHOWALL|  WS_CAPTION,NULL);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMusicDetectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMusicDetectionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_FILE_OPEN, &CMusicDetectionDlg::OnBnClickedFileOpen)
	ON_BN_CLICKED(IDC_RECORD, &CMusicDetectionDlg::OnBnClickedRecord)
	ON_BN_CLICKED(IDC_TSHAPE, &CMusicDetectionDlg::OnBnClickedTshape)
	ON_BN_CLICKED(IDC_FFTSHAPE, &CMusicDetectionDlg::OnBnClickedFftshape)
	ON_BN_CLICKED(IDC_ZEROCROSS, &CMusicDetectionDlg::OnBnClickedZerocross)
	ON_BN_CLICKED(IDC_OPENGL, &CMusicDetectionDlg::OnBnClickedOpengl)
	ON_BN_CLICKED(IDC_PLAY, &CMusicDetectionDlg::OnBnClickedPlay)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CRTMIDI, &CMusicDetectionDlg::OnBnClickedCrtmidi)
//	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_REFRESH, &CMusicDetectionDlg::OnBnClickedRefresh)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_INSERTDAT, &CMusicDetectionDlg::OnBnClickedInsertdat)
	ON_BN_CLICKED(IDC_WINEXEC, &CMusicDetectionDlg::OnBnClickedWinexec)
	ON_BN_CLICKED(IDC_MATCHER, &CMusicDetectionDlg::OnBnClickedMatcher)
END_MESSAGE_MAP()


// CMusicDetectionDlg 消息处理程序

BOOL CMusicDetectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMusicDetectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMusicDetectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CMusicDetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMusicDetectionDlg::OnBnClickedFileOpen()
{
	CFileDialog fileDlg(1,0,0,0,"波形文件(*.wav)|*.wav||");
	if (fileDlg.DoModal()==IDOK) {
		SetCurrentDirectory(mainPath);
		TriggerDocOpening(fileDlg.GetPathName());
	} else
		SetCurrentDirectory(mainPath);
}

float Pair[4096];
int size;
FILE *fwin;

void Trigger(float freq,float energy,bool next) {
	if (next) {
		fwrite(&size,4,1,fwin);
		fwrite(Pair,8,size,fwin);
		size=0;
	} else {
		Pair[size++]=freq;
		Pair[size++]=energy;
	}
}

const int tone[12]={261, 277, 293, 311, 329, 349, 370, 392, 415, 440, 466, 493};
const char rep[12]={'C', '#', 'D', '#', 'E', 'F', '#', 'G', '#', 'A', '#', 'B'};

int f(float freq) {
	#define get(x) ((tone[(x)%12]-10)*(1<<((x)/12))*0.5)

	float prev=get(0),that;
	if (freq>=prev) {
		for (int i=1;i<48;i++) {
			that=get(i);
			if (freq<that)
				return i-1;
			prev=that;
		}
	}
	return -1;
}

void CMusicDetectionDlg::TriggerDocOpening(const char *str) {
	CString buf;
	buf.Format("音乐检索 - %s",str);
	SetWindowText(buf);
	bool flag=pcm::loadPcmWave(str);
	buf.Format("%lu Hz",pcm::fmt.dwSamplesPerSec);
	GetDlgItem(IDC_EDIT1)->SetWindowText(buf);
	buf.Format("%lu bit",pcm::fmt.uiBitsPerSample);
	GetDlgItem(IDC_EDIT2)->SetWindowText(buf);
	GetDlgItem(IDC_EDIT3)->SetWindowText(pcm::fmt.wChannels!=1?"立体声":"单声道");
	unsigned long nSamples=pcm::getSampleCount();
	buf.Format("%lu Samples",nSamples);
	GetDlgItem(IDC_EDIT4)->SetWindowText(buf);
	buf.Format("%.2f s",float(nSamples)/pcm::fmt.dwSamplesPerSec);
	GetDlgItem(IDC_EDIT5)->SetWindowText(buf);
	*filename=0;
	if (!flag)
		MessageBox("当前波形文件不是单声道PCM格式的波形文件！");
	else {
		strcpy(filename,str);
		unsigned int length=pcm::getSampleCount();
		if (length<11025) {
			*filename=0;
			MessageBox("录音时间太短，没有得到足够的样本。");
			fclose(pcm::fp);
			return;
		}
		float *tDat=pcm::getPcmDatas();
		//pcm::preEmphasis(tDat,nSamples);
		FILE *ft=fopen("timeseq.bin","wb");
		fwrite(&length,4,1,ft);
		fwrite(tDat,4,length,ft);
		fclose(ft);
		size=0;
		fwin=fopen("winfft.bin","wb");
		float *fDat=pcm::getPcmFrequence(tDat,length,Trigger);
		fclose(fwin);
		FILE *ff=fopen("freqseq.bin","wb");
		fwrite(&length,4,1,ff);
		fwrite(fDat,4,length,ff);
		fclose(ff);

		unsigned char *sqnc=new unsigned char[length];
		for (int i=0;i<length;i++)
			sqnc[i]=f(fDat[i]);

		unsigned char *tones=NULL;
		unsigned int *lasts=NULL, lcnt=0;

		tones=new unsigned char[length];
		lasts=new unsigned int[length];
		lcnt=0;
		for (int j=0,i=1;i<=length;i++) {
			if (i==length || sqnc[j]!=sqnc[i]) {
				tones[lcnt]=sqnc[j];
				lasts[lcnt]=i-j;
				j=i;
				if (lcnt && lasts[lcnt]<2)
					continue;
				++lcnt;
			}
		}
		FILE *fn=fopen("toneseq.bin","wb");
		fwrite(&lcnt,4,1,fn);
		fwrite(tones,1,lcnt,fn);
		fwrite(lasts,4,lcnt,fn);
		fclose(fn);

		delete []sqnc;

		delete []tDat;
		delete []fDat;

		delete []tones;
		delete []lasts;
	}
	fclose(pcm::fp);
}

void CMusicDetectionDlg::OnBnClickedRecord()
{
	CString title;
	GetDlgItem(IDC_RECORD)->GetWindowText(title);
	if (title=="录音") {
		MCIWndNew(g_hWndMCIWnd,"waveaudio");
		MCIWndRecord(g_hWndMCIWnd);
		GetDlgItem(IDC_RECORD)->SetWindowText("停止");
	} else {
		MCIWndStop(g_hWndMCIWnd);
		MCIWndSave(g_hWndMCIWnd, "a");
		CopyFile("a","录音文件.wav",0);
		MCIWndClose(g_hWndMCIWnd);
		DeleteFile("a");
		TriggerDocOpening("录音文件.wav");
		GetDlgItem(IDC_RECORD)->SetWindowText("录音");
	}
}

void CMusicDetectionDlg::OnBnClickedTshape()
{
	if (!*filename)
		MessageBox("请先通过录音或载入本地文件方式打开一个单声道PCM音频文档！");
	else
		WinExec("tshape",1);
}

void CMusicDetectionDlg::OnBnClickedFftshape()
{
	if (!*filename)
		MessageBox("请先通过录音或载入本地文件方式打开一个单声道PCM音频文档！");
	else
		WinExec("winfft",1);
}

void CMusicDetectionDlg::OnBnClickedZerocross()
{
	if (!*filename)
		MessageBox("请先通过录音或载入本地文件方式打开一个单声道PCM音频文档！");
	else
		WinExec("zerocross",1);
}

void CMusicDetectionDlg::OnBnClickedOpengl()
{
	if (!*filename)
		MessageBox("请先通过录音或载入本地文件方式打开一个单声道PCM音频文档！");
	else
		WinExec("tonedtc",1);
}

void CMusicDetectionDlg::OnBnClickedPlay()
{
	if (!*filename)
		MessageBox("请先通过录音或载入本地文件方式打开一个单声道PCM音频文档！");
	else {
		CString buf;
		GetDlgItem(IDC_PLAY)->GetWindowText(buf);
		if (buf=="播放") {
			GetDlgItem(IDC_PLAY)->SetWindowText("停止");
			MCIWndOpen(g_hWndMCIWnd,filename,0);
			MCIWndPlay(g_hWndMCIWnd);
		} else {
			GetDlgItem(IDC_PLAY)->SetWindowText("播放");
			MCIWndStop(g_hWndMCIWnd);
			MCIWndClose(g_hWndMCIWnd);
		}
	}
}
void CMusicDetectionDlg::OnClose()
{
	DeleteFile("toneseq.bin");
	DeleteFile("freqseq.bin");
	DeleteFile("timeseq.bin");
	DeleteFile("winfft.bin");
	DeleteFile("matchdiff.bin");
	DeleteFile("filelist.bin");
	CDialog::OnClose();
}

void WriteMIDI(unsigned char *tones,unsigned int *lasts,unsigned length,const char *path) {
	FILE *fp=fopen(path, "wb");
	char head[]={0x4D,0x54,0x68,0x64,0x00,0x00,0x00,0x06,
		0x00,0x01,0x00,0x01,0x00,0x30,0x4D,0x54,
		0x72,0x6B},
		sgmt[]={0x00,0x90,0xFB,0x50,0x06,0x3C,0x00,0x00,0xFB},
		tail[]={0x00,0x00,0xFF,0x2F,0x00},
		play[]={0x00,0xC0,0x34};

	fwrite(head,sizeof(head),1,fp);

	int midiCnt=0;
	for (unsigned i=0;i<length;i++) {
			midiCnt+=lasts[i];
	}

	int finalLength = midiCnt * 9 + 8;
	char *lpvoid = (char*)&finalLength;
	fwrite(lpvoid+3,1,1,fp);
	fwrite(lpvoid+2,1,1,fp);
	fwrite(lpvoid+1,1,1,fp);
	fwrite(lpvoid+0,1,1,fp);
	fwrite(play,sizeof(play),1,fp);


	for (unsigned i=0;i<length;i++) {
		sgmt[2]=sgmt[8] = tones[i]+0x30;
		if (sgmt[2]==0x2F)
			sgmt[2]=sgmt[8]=0x00;
		for (unsigned j=lasts[i];j;--j) {
			fwrite(sgmt,sizeof(sgmt),1,fp);
			sgmt[1]=0x00;
		}
	}

	fwrite(tail,sizeof(tail),1,fp);

	fclose(fp);
}

void CMusicDetectionDlg::OnBnClickedCrtmidi()
{
	if (!*filename)
		MessageBox("请先通过录音或载入本地文件方式打开一个单声道PCM音频文档！");
	else {
		CFileDialog fileDlg(0,0,0,OFN_OVERWRITEPROMPT,"MIDI 序列文件(*.mid)|*.mid||");
		INT_PTR ans=fileDlg.DoModal();
		CString midiPath=fileDlg.GetPathName();
		if (midiPath.GetLength()>=4 && midiPath.Right(4)!=".mid")
			midiPath+=".mid";
		SetCurrentDirectory(mainPath);
		if (ans!=IDOK)
			return;
		FILE *fp=fopen("toneseq.bin","rb");
		int lcnt;
		fread(&lcnt,4,1,fp);
		unsigned char *tones=new unsigned char[lcnt];
		unsigned int *lasts=new unsigned int[lcnt];
		fread(tones,1,lcnt,fp);
		fread(lasts,4,lcnt,fp);
		fclose(fp);
		WriteMIDI(tones,lasts,lcnt,midiPath);
		MessageBox("成功生成MIDI文件！","消息提示",MB_ICONINFORMATION);
	}
}

void CMusicDetectionDlg::OnBnClickedRefresh()
{
	((CListBox*)GetDlgItem(IDC_MUSICLIST))->ResetContent();
	CFileFind fileFind;
	BOOL found=fileFind.FindFile(".\\database\\*");
	while (found) {
		found=fileFind.FindNextFile();
		CString dataFile=fileFind.GetFileName();
		if (dataFile.GetLength()>=4 && dataFile.Right(4)==".pcm" && dataFile.Find(' ')==-1)
			((CListBox*)GetDlgItem(IDC_MUSICLIST))->AddString(dataFile.Left(dataFile.GetLength()-4));
	}
	FILE *fp=fopen("filelist.bin","wb");
	int cnt=((CListBox*)GetDlgItem(IDC_MUSICLIST))->GetCount();
	CString item,buf;
	for (int i=0;i<cnt;i++) {
		((CListBox*)GetDlgItem(IDC_MUSICLIST))->GetText(i,item);
		buf.Format(".\\database\\%s.pcm",item);
		fprintf(fp,"%s\n",buf);
	}
	fclose(fp);

}

void CMusicDetectionDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
		OnBnClickedRefresh();
}

void CMusicDetectionDlg::OnBnClickedInsertdat()
{
	if (!*filename)
		MessageBox("请先通过录音或载入本地文件方式打开一个单声道PCM音频文档！");
	else {
		int start=0,end=strlen(filename);
		for (int i=end-1;i>=0;i--) {
			if (filename[i]=='\\') {
				start=i+1;
				break;
			}
		}
		for (int i=end-1;i>=start;i--) {
			if (filename[i]=='.') {
				end=i;
				break;
			}
		}
		CString fmt=filename;
		fmt=".\\database\\"+fmt.Mid(start,end-start)+".pcm";
		CopyFile("toneseq.bin",fmt,0);
		OnBnClickedRefresh();
		MessageBox("成功导入MIDI特征序列！","消息提示",MB_ICONINFORMATION);
	}
}

void CMusicDetectionDlg::OnBnClickedWinexec()
{
	CString currentDatabase;
	currentDatabase.Format("explorer \"%s\\database\\\"",mainPath);
	WinExec(currentDatabase,1);
}

void CMusicDetectionDlg::OnBnClickedMatcher()
{
	if (!*filename)
		MessageBox("请先通过录音或载入本地文件方式打开一个单声道PCM音频文档！");
	else
		WinExec("detectAudio",1);
}
