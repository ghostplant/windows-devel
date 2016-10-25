#include "resource.h"

#pragma once

// CResultDlg 对话框

class CResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResultDlg)
private:
	CWnd* pParent;

public:
	CResultDlg(CWnd* pParent = NULL);   // 标准构造函数
	CResultDlg(CString);
	virtual ~CResultDlg();

// 对话框数据
	enum { IDD = IDD_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void LexicalAnalyse(CString code, CListCtrl* ctrl);
//	int MoveToNext(int state,char c);

public:
	CString toString(int value) {
		CString str;
		str.Format("%d",value);
		return str;
	}

	bool isBlank(char ch) {
		return ch!=' '&& ch!='\t' && ch!='\n' && ch!='\r';
	}
	
	bool isIdStarter(char ch) {
		return ch=='_' || isalpha(ch);
	}

	bool isIdPart(char ch) {
		return ch=='_' || isalpha(ch) || isdigit(ch);
	}

	bool isLetter(char ch,char upcase) {
		return ch==upcase || ch==upcase+0x20;
	}

	bool isHexChar(char ch) {
		return isdigit(ch)|| ch>='a' && ch<='f'|| ch>='A' && ch<='F';
	}

};
