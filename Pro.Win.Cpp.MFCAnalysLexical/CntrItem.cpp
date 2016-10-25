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

// CntrItem.cpp : CLexicalAnalyserCntrItem 类的实现
//

#include "stdafx.h"
#include "LexicalAnalyser.h"

#include "LexicalAnalyserDoc.h"
#include "LexicalAnalyserView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLexicalAnalyserCntrItem 的实现

IMPLEMENT_SERIAL(CLexicalAnalyserCntrItem, CRichEditCntrItem, 0)

CLexicalAnalyserCntrItem::CLexicalAnalyserCntrItem(REOBJECT* preo, CLexicalAnalyserDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: 在此添加一次性构造代码
}

CLexicalAnalyserCntrItem::~CLexicalAnalyserCntrItem()
{
	// TODO: 在此处添加清理代码
}


// CLexicalAnalyserCntrItem 诊断

#ifdef _DEBUG
void CLexicalAnalyserCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CLexicalAnalyserCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

