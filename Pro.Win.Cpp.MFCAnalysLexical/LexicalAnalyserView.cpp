// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI")����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// LexicalAnalyserView.cpp : CLexicalAnalyserView ���ʵ��
//

#include "stdafx.h"
#include "ResultDlg.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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

// CLexicalAnalyserView ����/����

CLexicalAnalyserView::CLexicalAnalyserView()
{
	// TODO: �ڴ˴���ӹ������
	font=new CFont();
	font->CreatePointFont(100,"Courier New");
}

CLexicalAnalyserView::~CLexicalAnalyserView()
{
	delete font;
}

BOOL CLexicalAnalyserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CRichEditView::PreCreateWindow(cs);
}

void CLexicalAnalyserView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// ���ô�ӡ�߾�(720 � = 1/2 Ӣ��)
	SetMargins(CRect(720, 720, 720, 720));
}


// CLexicalAnalyserView ��ӡ


void CLexicalAnalyserView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLexicalAnalyserView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}


void CLexicalAnalyserView::OnDestroy()
{
	// ����ʱͣ�ô������
	// ʹ�ò����ͼʱ�ǳ���Ҫ 
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


// CLexicalAnalyserView ���

#ifdef _DEBUG
void CLexicalAnalyserView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CLexicalAnalyserView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CLexicalAnalyserDoc* CLexicalAnalyserView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLexicalAnalyserDoc)));
	return (CLexicalAnalyserDoc*)m_pDocument;
}
#endif //_DEBUG


// CLexicalAnalyserView ��Ϣ�������


int CLexicalAnalyserView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
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
	// TODO: �ڴ���������������
	CRichEditCtrl* rich=(CRichEditCtrl*)this;
	CString code,clean="";
	rich->GetWindowText(code);
	if (code.Right(1)!="\n")
		code+="\n";
	int state=0;
	for (int i=0;i<code.GetLength();i++) {
		if (state==0) { // ��Ч����״̬
			if (code[i]=='/')
				state=1;
			else {
				if (code[i]=='"')
					state=5;
				else if (code[i]=='\'')
					state=6;
				clean.AppendChar(code[i]);
			}
		} else if (state==1) { // ��б��״̬
			if (code[i]=='/')
				state=2;
			else if (code[i]=='*')
				state=3;
			else {
				clean.AppendChar('/');
				clean.AppendChar(code[i]);
				state=0;
			}
		} else if (state==2) { // ����ע��״̬
			if (code[i]=='\n') {
				clean.AppendChar('\n');
				state=0;
			}
		} else if (state==3) { // ����ע�Ϳ�ʼ״̬
			if (code[i]=='*')
				state=4;
		} else if (state==4) { // ����ע�Ϳ��ܽ���״̬
			if (code[i]=='/')
				state=0;
			else if (code[i]!='*')
				state=3;
		} else if (state==5) { // ˫�����ڲ�״̬
			clean.AppendChar(code[i]);
			if (code[i]=='"')
				state=0;
			else if (code[i]=='\\')
				state=7;
		} else if (state==6) { // �������ڲ�״̬
			clean.AppendChar(code[i]);
			if (code[i]=='\\')
				state=8;
			else
				state=9;
		} else if (state==7) { // ˫���ż�ת��״̬
			clean.AppendChar(code[i]);
			state=5;
		} else if (state==8) { // �����ż�ת��״̬
			clean.AppendChar(code[i]);
			state=9;
		} else if (state==9) { // �����Ű����״̬
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
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
	// TODO: �ڴ���������������
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
	SetWindowText("extern int printf(const char*,..); // C���Ա�׼�����������\n\nint main(int argc,char *argv[]) /* �����β�ֵ */ {\n\tlong long var=0x3fffffll;\n\tunsigned long uld=012348lu;\n\tprintf(\"\\xG\\76\\n\");\n\tint 4u=sizeof(unsigned long long int);\n\tchar *d=\"/*����ע�Ͳ�Ӧ�ñ����*/\";\n\tchar *c=\";\n\treturn 0a;\n}\n");
}
