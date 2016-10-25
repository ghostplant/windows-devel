// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// LexicalAnalyserView.h : CLexicalAnalyserView ��Ľӿ�
//
#include "LexicalAnalyserDoc.h"
#pragma once

class CLexicalAnalyserCntrItem;

class CLexicalAnalyserView : public CRichEditView
{
protected: // �������л�����
	CLexicalAnalyserView();
	DECLARE_DYNCREATE(CLexicalAnalyserView)

// ����
public:
	CLexicalAnalyserDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CLexicalAnalyserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnDestroy();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	CFont *font;
public:
//	afx_msg void OnCodetest();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnAnalyse();
	afx_msg void OnClean();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHighlight();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
};

#ifndef _DEBUG  // LexicalAnalyserView.cpp �еĵ��԰汾
inline CLexicalAnalyserDoc* CLexicalAnalyserView::GetDocument() const
{ return reinterpret_cast<CLexicalAnalyserDoc*>(m_pDocument); }
#endif

