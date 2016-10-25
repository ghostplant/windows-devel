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

// LexicalAnalyserDoc.cpp : CLexicalAnalyserDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 启用默认的 OLE 容器实现
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()


// CLexicalAnalyserDoc 构造/析构

CLexicalAnalyserDoc::CLexicalAnalyserDoc()
{
	// TODO: 在此添加一次性构造代码

}

CLexicalAnalyserDoc::~CLexicalAnalyserDoc()
{
}

BOOL CLexicalAnalyserDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

CRichEditCntrItem* CLexicalAnalyserDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CLexicalAnalyserCntrItem(preo, const_cast<CLexicalAnalyserDoc*>(this));
}




// CLexicalAnalyserDoc 序列化

void CLexicalAnalyserDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}

	// 调用基类 CRichEditDoc 将启用
	//  容器文档的 COleClientItem 对象的序列化。
	// TODO: 如果作为文本进行序列化，则设置 CRichEditDoc::m_bRTF = FALSE
	CRichEditDoc::Serialize(ar);
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CLexicalAnalyserDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
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

// 搜索处理程序的支持
void CLexicalAnalyserDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
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

// CLexicalAnalyserDoc 诊断

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


// CLexicalAnalyserDoc 命令


//BOOL CLexicalAnalyserDoc::OnSaveDocument(LPCTSTR lpszPathName)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	return TRUE;
//}


//BOOL CLexicalAnalyserDoc::OnOpenDocument(LPCTSTR lpszPathName)
//{
//	if (!CRichEditDoc::OnOpenDocument(lpszPathName))
//		return FALSE;
//
//	// TODO:  在此添加您专用的创建代码
//	CFile file;
//
//	file.Close();
//	return TRUE;
//}


BOOL CLexicalAnalyserDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类
	CFile file;
	if (!file.Open(lpszPathName,CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("无法保存该文件，请确保文件是否允许访问！");
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
	// TODO:  在此添加您专用的创建代码
	CFile file;
	if (!file.Open(lpszPathName,CFile::modeRead))
	{
		AfxMessageBox("无法打开该文件，请确保该文件是否存在或者允许访问！");
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
