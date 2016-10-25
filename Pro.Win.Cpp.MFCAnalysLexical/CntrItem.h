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

// CntrItem.h : CLexicalAnalyserCntrItem ��Ľӿ�
//

#pragma once

class CLexicalAnalyserDoc;
class CLexicalAnalyserView;

class CLexicalAnalyserCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CLexicalAnalyserCntrItem)

// ���캯��
public:
	CLexicalAnalyserCntrItem(REOBJECT* preo = NULL, CLexicalAnalyserDoc* pContainer = NULL);
		// ע��: ���� pContainer Ϊ NULL ������ IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE Ҫ������д���
		//  �����Ĺ��캯����OLE ��ͨ������
		//  �� NULL �ĵ�ָ�빹���

// ����
public:
	CLexicalAnalyserDoc* GetDocument()
		{ return reinterpret_cast<CLexicalAnalyserDoc*>(CRichEditCntrItem::GetDocument()); }
	CLexicalAnalyserView* GetActiveView()
		{ return reinterpret_cast<CLexicalAnalyserView*>(CRichEditCntrItem::GetActiveView()); }

// ʵ��
public:
	~CLexicalAnalyserCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

