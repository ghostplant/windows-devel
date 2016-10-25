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

// LexicalAnalyserDoc.cpp : CLexicalAnalyserDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "LexicalAnalyser.h"
#endif

#include "LexicalAnalyserDoc.h"
#include "CntrItem.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLexicalAnalyserDoc

IMPLEMENT_DYNCREATE(CLexicalAnalyserDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CLexicalAnalyserDoc, CRichEditDoc)
	// ����Ĭ�ϵ� OLE ����ʵ��
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()


// CLexicalAnalyserDoc ����/����

CLexicalAnalyserDoc::CLexicalAnalyserDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CLexicalAnalyserDoc::~CLexicalAnalyserDoc()
{
}

BOOL CLexicalAnalyserDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

CRichEditCntrItem* CLexicalAnalyserDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CLexicalAnalyserCntrItem(preo, const_cast<CLexicalAnalyserDoc*>(this));
}




// CLexicalAnalyserDoc ���л�

void CLexicalAnalyserDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}

	// ���û��� CRichEditDoc ������
	//  �����ĵ��� COleClientItem ��������л���
	// TODO: �����Ϊ�ı��������л��������� CRichEditDoc::m_bRTF = FALSE
	CRichEditDoc::Serialize(ar);
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CLexicalAnalyserDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CLexicalAnalyserDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CLexicalAnalyserDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CLexicalAnalyserDoc ���

#ifdef _DEBUG
void CLexicalAnalyserDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CLexicalAnalyserDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// CLexicalAnalyserDoc ����


//BOOL CLexicalAnalyserDoc::OnSaveDocument(LPCTSTR lpszPathName)
//{
//	// TODO: �ڴ����ר�ô����/����û���
//	return TRUE;
//}


//BOOL CLexicalAnalyserDoc::OnOpenDocument(LPCTSTR lpszPathName)
//{
//	if (!CRichEditDoc::OnOpenDocument(lpszPathName))
//		return FALSE;
//
//	// TODO:  �ڴ������ר�õĴ�������
//	CFile file;
//
//	file.Close();
//	return TRUE;
//}


BOOL CLexicalAnalyserDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: �ڴ����ר�ô����/����û���
	CFile file;
	if (!file.Open(lpszPathName,CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("�޷�������ļ�����ȷ���ļ��Ƿ�������ʣ�");
		return FALSE;
	}

	CString str;
	POSITION pos=this->GetFirstViewPosition();
	this->GetNextView(pos)->GetWindowText(str);
	file.Write(str.GetBuffer(),str.GetLength());
	this->SetModifiedFlag(0);
	file.Close();
	return TRUE;
}


BOOL CLexicalAnalyserDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CRichEditDoc::OnOpenDocument(lpszPathName))
		return FALSE;
	// TODO:  �ڴ������ר�õĴ�������
	CFile file;
	if (!file.Open(lpszPathName,CFile::modeRead))
	{
		AfxMessageBox("�޷��򿪸��ļ�����ȷ�����ļ��Ƿ���ڻ���������ʣ�");
		return FALSE;
	}
	int length=(int)file.SeekToEnd();
	file.SeekToBegin();
	TCHAR *buffer=new TCHAR[length+1];
	file.Read(buffer,length);
	buffer[length]=0;
	POSITION pos=this->GetFirstViewPosition();
	this->GetNextView(pos)->SetWindowText(buffer);
	file.Close();
	return TRUE;
}
