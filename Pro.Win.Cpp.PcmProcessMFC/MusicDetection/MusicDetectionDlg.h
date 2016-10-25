// MusicDetectionDlg.h : 头文件
//

#pragma once


// CMusicDetectionDlg 对话框
class CMusicDetectionDlg : public CDialog
{
// 构造
public:
	CMusicDetectionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MUSICDETECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
