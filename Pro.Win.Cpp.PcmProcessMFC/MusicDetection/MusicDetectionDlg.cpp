// MusicDetectionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicDetection.h"
#include "MusicDetectionDlg.h"

#include "pcmwave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMusicDetectionDlg �Ի���




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


// CMusicDetectionDlg ��Ϣ�������

BOOL CMusicDetectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMusicDetectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CMusicDetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMusicDetectionDlg::OnBnClickedFileOpen()
{
	CFileDialog fileDlg(1,0,0,0,"�����ļ�(*.wav)|*.wav||");
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
	buf.Format("���ּ��� - %s",str);
	SetWindowText(buf);
	bool flag=pcm::loadPcmWave(str);
	buf.Format("%lu Hz",pcm::fmt.dwSamplesPerSec);
	GetDlgItem(IDC_EDIT1)->SetWindowText(buf);
	buf.Format("%lu bit",pcm::fmt.uiBitsPerSample);
	GetDlgItem(IDC_EDIT2)->SetWindowText(buf);
	GetDlgItem(IDC_EDIT3)->SetWindowText(pcm::fmt.wChannels!=1?"������":"������");
	unsigned long nSamples=pcm::getSampleCount();
	buf.Format("%lu Samples",nSamples);
	GetDlgItem(IDC_EDIT4)->SetWindowText(buf);
	buf.Format("%.2f s",float(nSamples)/pcm::fmt.dwSamplesPerSec);
	GetDlgItem(IDC_EDIT5)->SetWindowText(buf);
	*filename=0;
	if (!flag)
		MessageBox("��ǰ�����ļ����ǵ�����PCM��ʽ�Ĳ����ļ���");
	else {
		strcpy(filename,str);
		unsigned int length=pcm::getSampleCount();
		if (length<11025) {
			*filename=0;
			MessageBox("¼��ʱ��̫�̣�û�еõ��㹻��������");
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
	if (title=="¼��") {
		MCIWndNew(g_hWndMCIWnd,"waveaudio");
		MCIWndRecord(g_hWndMCIWnd);
		GetDlgItem(IDC_RECORD)->SetWindowText("ֹͣ");
	} else {
		MCIWndStop(g_hWndMCIWnd);
		MCIWndSave(g_hWndMCIWnd, "a");
		CopyFile("a","¼���ļ�.wav",0);
		MCIWndClose(g_hWndMCIWnd);
		DeleteFile("a");
		TriggerDocOpening("¼���ļ�.wav");
		GetDlgItem(IDC_RECORD)->SetWindowText("¼��");
	}
}

void CMusicDetectionDlg::OnBnClickedTshape()
{
	if (!*filename)
		MessageBox("����ͨ��¼�������뱾���ļ���ʽ��һ��������PCM��Ƶ�ĵ���");
	else
		WinExec("tshape",1);
}

void CMusicDetectionDlg::OnBnClickedFftshape()
{
	if (!*filename)
		MessageBox("����ͨ��¼�������뱾���ļ���ʽ��һ��������PCM��Ƶ�ĵ���");
	else
		WinExec("winfft",1);
}

void CMusicDetectionDlg::OnBnClickedZerocross()
{
	if (!*filename)
		MessageBox("����ͨ��¼�������뱾���ļ���ʽ��һ��������PCM��Ƶ�ĵ���");
	else
		WinExec("zerocross",1);
}

void CMusicDetectionDlg::OnBnClickedOpengl()
{
	if (!*filename)
		MessageBox("����ͨ��¼�������뱾���ļ���ʽ��һ��������PCM��Ƶ�ĵ���");
	else
		WinExec("tonedtc",1);
}

void CMusicDetectionDlg::OnBnClickedPlay()
{
	if (!*filename)
		MessageBox("����ͨ��¼�������뱾���ļ���ʽ��һ��������PCM��Ƶ�ĵ���");
	else {
		CString buf;
		GetDlgItem(IDC_PLAY)->GetWindowText(buf);
		if (buf=="����") {
			GetDlgItem(IDC_PLAY)->SetWindowText("ֹͣ");
			MCIWndOpen(g_hWndMCIWnd,filename,0);
			MCIWndPlay(g_hWndMCIWnd);
		} else {
			GetDlgItem(IDC_PLAY)->SetWindowText("����");
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
		MessageBox("����ͨ��¼�������뱾���ļ���ʽ��һ��������PCM��Ƶ�ĵ���");
	else {
		CFileDialog fileDlg(0,0,0,OFN_OVERWRITEPROMPT,"MIDI �����ļ�(*.mid)|*.mid||");
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
		MessageBox("�ɹ�����MIDI�ļ���","��Ϣ��ʾ",MB_ICONINFORMATION);
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
		MessageBox("����ͨ��¼�������뱾���ļ���ʽ��һ��������PCM��Ƶ�ĵ���");
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
		MessageBox("�ɹ�����MIDI�������У�","��Ϣ��ʾ",MB_ICONINFORMATION);
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
		MessageBox("����ͨ��¼�������뱾���ļ���ʽ��һ��������PCM��Ƶ�ĵ���");
	else
		WinExec("detectAudio",1);
}
