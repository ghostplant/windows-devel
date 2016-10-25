// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI")。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// LexicalAnalyserView.cpp : CLexicalAnalyserView 类的实现
//

#include "stdafx.h"
#include "ResultDlg.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LexicalAnalyser.h"
#endif

#include "LexicalAnalyserDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "LexicalAnalyserView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLexicalAnalyserView

IMPLEMENT_DYNCREATE(CLexicalAnalyserView, CRichEditView)

BEGIN_MESSAGE_MAP(CLexicalAnalyserView, CRichEditView)
	ON_WM_DESTROY()
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLexicalAnalyserView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
//	ON_COMMAND(ID_CODETEST, &CLexicalAnalyserView::OnCodetest)
//	ON_WM_KEYDOWN()
//	ON_WM_KEYUP()
//ON_WM_KEYDOWN()
ON_COMMAND(ID_ANALYSE, &CLexicalAnalyserView::OnAnalyse)
ON_COMMAND(ID_CLEAN, &CLexicalAnalyserView::OnClean)
ON_WM_KEYDOWN()
ON_COMMAND(ID_HIGHLIGHT, &CLexicalAnalyserView::OnHighlight)
ON_COMMAND(ID_BUTTON2, &CLexicalAnalyserView::OnButton2)
ON_COMMAND(ID_BUTTON3, &CLexicalAnalyserView::OnButton3)
END_MESSAGE_MAP()

// CLexicalAnalyserView 构造/析构

CLexicalAnalyserView::CLexicalAnalyserView()
{
	// TODO: 在此处添加构造代码
	font=new CFont();
	font->CreatePointFont(100,"Courier New");
}

CLexicalAnalyserView::~CLexicalAnalyserView()
{
	delete font;
}

BOOL CLexicalAnalyserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CRichEditView::PreCreateWindow(cs);
}

void CLexicalAnalyserView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// 设置打印边距(720 缇 = 1/2 英寸)
	SetMargins(CRect(720, 720, 720, 720));
}


// CLexicalAnalyserView 打印


void CLexicalAnalyserView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLexicalAnalyserView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}


void CLexicalAnalyserView::OnDestroy()
{
	// 析构时停用此项；这在
	// 使用拆分视图时非常重要 
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CRichEditView::OnDestroy();
}


void CLexicalAnalyserView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLexicalAnalyserView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLexicalAnalyserView 诊断

#ifdef _DEBUG
void CLexicalAnalyserView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CLexicalAnalyserView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CLexicalAnalyserDoc* CLexicalAnalyserView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLexicalAnalyserDoc)));
	return (CLexicalAnalyserDoc*)m_pDocument;
}
#endif //_DEBUG


// CLexicalAnalyserView 消息处理程序


int CLexicalAnalyserView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	OnKeyDown(0,0,0);
	return 0;
}



void CLexicalAnalyserView::OnAnalyse()
{
	CResultDlg res(this);
	res.DoModal();
}


void CLexicalAnalyserView::OnClean()
{
	// TODO: 在此添加命令处理程序代码
	CRichEditCtrl* rich=(CRichEditCtrl*)this;
	CString code,clean="";
	rich->GetWindowText(code);
	if (code.Right(1)!="\n")
		code+="\n";
	int state=0;
	for (int i=0;i<code.GetLength();i++) {
		if (state==0) { // 有效代码状态
			if (code[i]=='/')
				state=1;
			else {
				if (code[i]=='"')
					state=5;
				else if (code[i]=='\'')
					state=6;
				clean.AppendChar(code[i]);
			}
		} else if (state==1) { // 单斜杠状态
			if (code[i]=='/')
				state=2;
			else if (code[i]=='*')
				state=3;
			else {
				clean.AppendChar('/');
				clean.AppendChar(code[i]);
				state=0;
			}
		} else if (state==2) { // 单行注释状态
			if (code[i]=='\n') {
				clean.AppendChar('\n');
				state=0;
			}
		} else if (state==3) { // 多行注释开始状态
			if (code[i]=='*')
				state=4;
		} else if (state==4) { // 多行注释可能结束状态
			if (code[i]=='/')
				state=0;
			else if (code[i]!='*')
				state=3;
		} else if (state==5) { // 双引号内部状态
			clean.AppendChar(code[i]);
			if (code[i]=='"')
				state=0;
			else if (code[i]=='\\')
				state=7;
		} else if (state==6) { // 单引号内部状态
			clean.AppendChar(code[i]);
			if (code[i]=='\\')
				state=8;
			else
				state=9;
		} else if (state==7) { // 双引号加转义状态
			clean.AppendChar(code[i]);
			state=5;
		} else if (state==8) { // 单引号加转义状态
			clean.AppendChar(code[i]);
			state=9;
		} else if (state==9) { // 单引号半结束状态
			clean.AppendChar(code[i]);
			if (code[i]=='\'')
				state=0;
			else
				state=9;
		}
	}
	rich->SetWindowText(clean);
}


void CLexicalAnalyserView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	this->SetFont(font);

	CHARFORMAT cf;
	((CRichEditCtrl*)this)->GetDefaultCharFormat(cf);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.crTextColor = RGB(0,0,0);
	((CRichEditCtrl*)this)->SetDefaultCharFormat(cf);

	((CRichEditCtrl*)this)->SetBackgroundColor(false,RGB(235,245,255));

	CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CLexicalAnalyserView::OnHighlight()
{
	// TODO: 在此添加命令处理程序代码
	OnClean();

	CString code;
	GetWindowText(code);

	CResultDlg res(this);
	res.LexicalAnalyse(code,NULL);
}


void CLexicalAnalyserView::OnButton2()
{
	SetWindowText("#include <cstdio>\n#include <cstring>\n#include <cstdlib>\n#include <cmath>\n#include <algorithm>\n#include <map>\n#include <string>\n#include <set>\n\nusing namespace std;\n\nint n, m;\n\nstruct node {\n    int r, o;\n    long long v;\n} a[2050];\n\nlong long bag[20050];\n\nbool cmp(const node &a, const node &b) {\n    if (a.o != b.o)\n        return a.o < b.o;\n    if (a.v != b.v)\n        return a.v > b.v;\n    return a.r < b.r;\n}\n\nint main() {\n    int tc, i, j;\n    scanf(\"%d\", &tc);\n    while (tc--) {\n        scanf(\"%d%d\", &n, &m);\n        for (i = 0; i < n; i++)\n            scanf(\"%d\", &a[i].r);\n        for (i = 0; i < n; i++)\n            scanf(\"%d\", &a[i].o);\n        for (i = 0; i < n; i++)\n            scanf(\"%lld\", &a[i].v);\n        memset(bag, -1, sizeof (bag));\n        *bag = 0;\n        long long ans = 0;\n        sort(a, a + n, cmp);\n        for (i = 0; i < n; i++) {\n            for (j = m; j >= 0; j--)\n                if ((~bag[j]) && j + a[i].o <= m) {\n                    bag[j + a[i].o] = max(bag[j + a[i].o], bag[j] + a[i].v);\n                    if (j + a[i].r <= m)\n                        ans = max(ans, bag[j + a[i].o]);\n                }\n        }\n        printf(\"%lld\\n\", ans);\n    }\n    return 0;\n}\n");
}


void CLexicalAnalyserView::OnButton3()
{
	SetWindowText("extern int printf(const char*,..); // C语言标准输出函数声明\n\nint main(int argc,char *argv[]) /* 传入形参值 */ {\n\tlong long var=0x3fffffll;\n\tunsigned long uld=012348lu;\n\tprintf(\"\\xG\\76\\n\");\n\tint 4u=sizeof(unsigned long long int);\n\tchar *d=\"/*串内注释不应该被清除*/\";\n\tchar *c=\";\n\treturn 0a;\n}\n");
}
