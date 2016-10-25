// MusicDetectionDlg.h : ͷ�ļ�
//

#pragma once


// CMusicDetectionDlg �Ի���
class CMusicDetectionDlg : public CDialog
{
// ����
public:
	CMusicDetectionDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MUSICDETECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HWND g_hWndMCIWnd;
	char mainPath[MAX_PATH],filename[MAX_PATH];
public:
	afx_msg void OnBnClickedFileOpen();
public:
	afx_msg void OnBnClickedRecord();
	void TriggerDocOpening(const char *);
public:
	afx_msg void OnBnClickedTshape();
public:
	afx_msg void OnBnClickedFftshape();
public:
	afx_msg void OnBnClickedZerocross();
public:
	afx_msg void OnBnClickedOpengl();
public:
	afx_msg void OnBnClickedPlay();
public:
	afx_msg void OnClose();
public:
	afx_msg void OnBnClickedCrtmidi();
public:
	afx_msg void OnBnClickedRefresh();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnBnClickedInsertdat();
public:
	afx_msg void OnBnClickedWinexec();
public:
	afx_msg void OnBnClickedMatcher();
};
