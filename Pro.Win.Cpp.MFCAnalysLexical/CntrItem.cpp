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

// CntrItem.cpp : CLexicalAnalyserCntrItem ���ʵ��
//

#include "stdafx.h"
#include "LexicalAnalyser.h"

#include "LexicalAnalyserDoc.h"
#include "LexicalAnalyserView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLexicalAnalyserCntrItem ��ʵ��

IMPLEMENT_SERIAL(CLexicalAnalyserCntrItem, CRichEditCntrItem, 0)

CLexicalAnalyserCntrItem::CLexicalAnalyserCntrItem(REOBJECT* preo, CLexicalAnalyserDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: �ڴ����һ���Թ������
}

CLexicalAnalyserCntrItem::~CLexicalAnalyserCntrItem()
{
	// TODO: �ڴ˴�����������
}


// CLexicalAnalyserCntrItem ���

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

