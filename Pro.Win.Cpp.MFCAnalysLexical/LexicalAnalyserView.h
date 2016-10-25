// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// LexicalAnalyserView.h : CLexicalAnalyserView 类的接口
//
#include "LexicalAnalyserDoc.h"
#pragma once

class CLexicalAnalyserCntrItem;

class CLexicalAnalyserView : public CRichEditView
{
protected: // 仅从序列化创建
	CLexicalAnalyserView();
	DECLARE_DYNCREATE(CLexicalAnalyserView)

// 特性
public:
	CLexicalAnalyserDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// 实现
public:
	virtual ~CLexicalAnalyserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // LexicalAnalyserView.cpp 中的调试版本
inline CLexicalAnalyserDoc* CLexicalAnalyserView::GetDocument() const
{ return reinterpret_cast<CLexicalAnalyserDoc*>(m_pDocument); }
#endif

