// Result.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LexicalAnalyser.h"
#include "ResultDlg.h"
#include "afxdialogex.h"
#include "LexicalAnalyserView.h"

#include "stdio.h"
#include "vector"
#include "set"

using std::vector;
using std::set;

#define err(x) ("��������Ϣ��"+(x))

const char *key_word[]={"auto","static","register","volatile","char","float","int","long","signed","unsigned",
	"void","double","short","char","float","int","long","signed","unsigned","void","double","short","enum",
	"struct","union","typedef","return","break","continue","goto","const","extern","if","else","switch",
	"default","case","do","while","for","sizeof"};

// CResultDlg �Ի���

IMPLEMENT_DYNAMIC(CResultDlg, CDialogEx)
	
CResultDlg::CResultDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResultDlg::IDD, pParent)
{
	this->pParent=pParent;
}

CResultDlg::~CResultDlg()
{
}

void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CResultDlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CResultDlg::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CResultDlg ��Ϣ�������


int CResultDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO:  �ڴ������ר�õĴ�������
	

	return 0;
}


void CResultDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CFileDialog fd(0,"csv",0,0,"���ŷָ��ı� (*.csv)|*.csv|�����ļ� (*.*)|*.*||");
	if (fd.DoModal()==IDOK) {
		CFile file;
		file.Open(fd.GetFileName(),CFile::modeCreate|CFile::modeWrite);
		CListCtrl *ctrl=((CListCtrl*)GetDlgItem(IDC_GRIDLIST));
		int cnt=ctrl->GetItemCount();
		for (int i=0;i<cnt;i++) {
			for (int j=0;j<4;j++) {
				CString item=ctrl->GetItemText(i,j);
				file.Write(item.GetBuffer(),item.GetLength());
				file.Write(",",1);
			}
			file.Write("\r\n",2);
		}
		file.Close();
		MessageBox("����CSV�����ļ��ɹ���","��Ϣ",MB_ICONINFORMATION);

		CDialogEx::OnOK();
	}
}


void CResultDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	if (bShow) {

		CListCtrl *m_list=((CListCtrl*)GetDlgItem(IDC_GRIDLIST)); 
	
		LONG lStyle;
		  lStyle = GetWindowLong(m_list->m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
		  lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
		  lStyle |= LVS_REPORT; //����style
		  SetWindowLong(m_list->m_hWnd, GWL_STYLE, lStyle);//����style

		DWORD dwStyle = m_list->GetExtendedStyle();
		  dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
		  dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
		  m_list->SetExtendedStyle(dwStyle); //������չ���
		  
		  m_list->InsertColumn( 0, "����", LVCFMT_LEFT, 50 );//������
		  m_list->InsertColumn( 1, "����", LVCFMT_LEFT, 50 );//������
		  m_list->InsertColumn( 2, "����", LVCFMT_LEFT, 250 );//������
		  m_list->InsertColumn( 3, "ֵ", LVCFMT_LEFT, 120 );//������
		  CString code;
		  ((CLexicalAnalyserView*)pParent)->OnClean();
		  pParent->GetWindowText(code);
		  LexicalAnalyse(code,m_list);
	}
}

void CResultDlg::LexicalAnalyse(CString code, CListCtrl* ctrl)
{
	((CRichEditCtrl*)pParent)->HideSelection(true,false);

	static CString desc[256];
	desc['?']="$QUESTION(�ʺ������)";
	desc[':']="$QUOTE(���������)";
	desc['~']="$WAVE(��λȡ�������)";
	desc[',']="$COMMA(���������)";
	desc['(']="$LEFT_BITTER_BRACKET(��С���Ž��)";
	desc[')']="$RIGHT_BITTER_BRACKET(��С���Ž��)";
	desc['[']="$LEFT_MIDDLE_BRACKET(�������Ž��)";
	desc[']']="$RIGHT_MIDDLE_BRACKET(�������Ž��)";
	desc['{']="$LEFT_LARGE_BRACKET(������Ž��)";
	desc['}']="$RIGHT_LARGE_BRACKET(�Ҵ����Ž��)";
	desc[';']="$SEMICOLON(�ֺŽ��)";
	desc['#']="$MACRO(���־���)";

	set<CString> keywords;
	for (int i=0;i<sizeof(key_word)/sizeof(*key_word);i++)
		keywords.insert(key_word[i]);

	struct Message {
		Message(CString a,CString b,CString c,CString d="") {
			line=a,column=b,type=c,value=d;
		}
		CString line,column,type,value;
	};
	
	vector<Message> words,error;
	int state, ch,prev=0,ind,lncount=0;
	
	while (ind=code.Find("\n",prev),~ind) {
		state=0;
		CString buffer="",line=code.Mid(prev,ind-prev),lineNumber;
		lineNumber.Format("%d",++lncount);
		for (int i=0;i<line.GetLength();i++) {
			ch=line[i];
			switch (state) {
				case 0:
					if (isBlank(ch)) {
						buffer.AppendChar(ch);
						if (isdigit(ch) && ch!='0')
							state=1;
						else if (ch=='.')
							state=9;
						else if (isIdStarter(ch))
							state=7;
						else if (ch=='+')
							state=8;
						else if (ch=='-')
							state=10;
						else if (ch=='|')
							state=11;
						else if (ch=='&')
							state=12;
						else if (ch=='*' || ch=='/' || ch=='%' || ch=='^')
							state=13;
						else if (ch=='\'')
							state=17;
						else if (ch=='0')
							state=36;
						else if (ch=='=' || ch=='!')
							state=25;
						else if (ch=='>')
							state=26;
						else if (ch=='<')
							state=28;
						else if (ch=='"')
							state=30;
						else if (ch=='?' || ch==':' || ch=='~' || ch=='(' || ch==')' || ch=='[' || ch==']' || ch=='{' || ch=='}' || ch==',' || ch==';' || ch=='#') {
							CString charactor;
							charactor+=(char)ch;
							words.push_back(Message(lineNumber,toString(i+1),desc[ch],charactor));
							buffer="", state=0;
						} else {
							CString str;
							str.Format("�޷�ʶ���ַ�'%c'��",ch);
							error.push_back(Message(lineNumber,toString(i+1),err(str)));
							state=-1;
						}
					}
					break;
				case 1:
					if (isdigit(ch)) {
						buffer.AppendChar(ch);
						state=1;
					} else if (ch=='.') {
						buffer.AppendChar(ch);
						state=2;
					} else if (isLetter(ch,'E')) {
						buffer.AppendChar(ch);
						state=3;
					} else if (isLetter(ch,'F')) {
						state=44;
					} else if (isLetter(ch,'U')) {
						state=45;
					} else if (isLetter(ch,'L'))
						state=14;
					else if (!isIdStarter(ch)) {
						CString intValue;
						intValue.Format("%d",atoi(buffer));
						words.push_back(Message(lineNumber,toString(i+1-buffer.GetLength()),"$CONST_INT(���ͳ���)",intValue));
						state=0,buffer="",--i;
					} else {
						CString str;
						str.Format("���ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 2:
					if (isdigit(ch)) {
						buffer.AppendChar(ch);
						state=2;
					} else if (isLetter(ch,'F'))
						state=44;
					else if (isLetter(ch,'L'))
						state=16;
					else if (isLetter(ch,'E')) {
						buffer.AppendChar(ch);
						state=3;
					} else if (isIdStarter(ch)) {
						CString str;
						str.Format("������%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					} else {
						CString doubleValue;
						doubleValue.Format("%g",atof(buffer));
						if (doubleValue.Find(".")==-1)
							doubleValue+=".0";
						words.push_back(Message(lineNumber,toString(i+1-buffer.GetLength()),"$CONST_DOUBLE(˫���ȸ��㳣��)",doubleValue));
						state=0,buffer="",--i;
					}
					break;
				case 3:
					if (isdigit(ch)) {
						buffer.AppendChar(ch);
						state=5;
					} else if (ch=='+' || ch=='-') {
						buffer.AppendChar(ch);
						state=4;
					} else {
						CString str;
						str.Format("������%s�������ִ��ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 4:
					if (isdigit(ch)) {
						buffer.AppendChar(ch);
						state=5;
					} else {
						CString str;
						str.Format("������%s�������ִ��ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 5:
					if (isdigit(ch)) {
						buffer.AppendChar(ch);
						state=5;
					} else if (isLetter(ch,'L'))
						state=16;
					else if (isLetter(ch,'F'))
						state=44;
					else if (isIdStarter(ch)) {
						CString str;
						str.Format("������%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					} else {
						CString doubleValue;
						doubleValue.Format("%g",atof(buffer));
						if (doubleValue.Find(".")==-1)
							doubleValue+=".0";
						words.push_back(Message(lineNumber,toString(i+1-buffer.GetLength()),"$CONST_DOUBLE(˫���ȸ��㳣��)",doubleValue));
						state=0,buffer="",--i;
					}
					break;
				case 6:
					if (isHexChar(ch)) {
						buffer.AppendChar(ch);
						state=38;
					} else {
						CString str;
						str.Format("���ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 7:
					if (isIdPart(ch)) {
						buffer.AppendChar(ch);
						state=7;
					} else if (keywords.find(buffer)!=keywords.end()) {
						words.push_back(Message(lineNumber,toString(i+1),"$KEYWORD(�ؼ���)",buffer));
						CString sub;
						int ind=((CRichEditCtrl*)pParent)->LineIndex(lncount-1);
						((CRichEditCtrl*)pParent)->SetSel(ind+i-buffer.GetLength(),ind+i);

						CHARFORMAT cf;
						((CRichEditCtrl*)this)->GetDefaultCharFormat(cf);
						cf.dwMask = CFM_COLOR;
						cf.dwEffects &= ~CFE_AUTOCOLOR;
						cf.crTextColor = RGB(0,0,255);
						((CRichEditCtrl*)this)->SetDefaultCharFormat(cf);

						((CRichEditCtrl*)pParent)->SetSelectionCharFormat(cf);
						state=0,buffer="",--i;
					} else {
						words.push_back(Message(lineNumber,toString(i+1),"$IDENTIFIER(��ʶ��)",buffer));
						state=0,buffer="",--i;
					}
					break;
				case 8:
					buffer.Format("%d",i+1);
					if (ch=='=') {
						words.push_back(Message(lineNumber,buffer,"$SELF_PLUS(�Լ������)","+="));
						state=0,buffer="";
					} else if (ch=='+') {
						words.push_back(Message(lineNumber,buffer,"$SELF_PLUS_ONE(���������)","++"));
						state=0,buffer="";
					} else {
						words.push_back(Message(lineNumber,toString(i+1),"$PLUS(�ӷ������)"));
						state=0,buffer="",--i;
					}
					break;
				case 9:
					if (ch=='.') {
						buffer.Format("%d",i+1);
						words.push_back(Message(lineNumber,buffer,"$OMIT_ARG(�β�ʡ�Խ��)",".."));
						state=0,buffer="";
					} else if (isdigit(ch)) {
						buffer.AppendChar(ch);
						state=2;
					}else {
						words.push_back(Message(lineNumber,toString(i+1),"$MEMBER_POINT(ȡ������Ա�����)","."));
						state=0,buffer="",--i;
					}
					break;
				case 10:
					buffer.Format("%d",i+1);
					if (ch=='>') {
						words.push_back(Message(lineNumber,buffer,"$MEMBER_ARROW(ȡָ���Ա�����)","->"));
						state=0,buffer="";
					} else if (ch=='=') {
						words.push_back(Message(lineNumber,buffer,"$SELF_SUBTACT(�Լ������)","-="));
						state=0,buffer="";
					}else if (ch=='-') {
						words.push_back(Message(lineNumber,buffer,"$SELF_SUBTACT_ONE(�Խ������)","--"));
						state=0,buffer="";
					} else {
						words.push_back(Message(lineNumber,toString(i+1),"$SUBTRACT(���������)","-"));
						state=0,buffer="",--i;
					}
					break;
				case 11:
					buffer.Format("%d",i+1);
					if (ch=='|') {
						words.push_back(Message(lineNumber,buffer,"$LOGIC_OR(�߼��������)","||"));
						state=0,buffer="";
					} else if (ch=='=') {
						words.push_back(Message(lineNumber,buffer,"$SELF_BIT_OR(��λ����������)","|="));
						state=0,buffer="";
					} else {
						words.push_back(Message(lineNumber,toString(i+1),"$BIT_OR(��λ�������)","|"));
						state=0,buffer="",--i;
					}
					break;
				case 12:
					buffer.Format("%d",i+1);
					if (ch=='&') {
						words.push_back(Message(lineNumber,buffer,"$LOGIC_AND(�߼��������)","&&"));
						state=0,buffer="";
					} else if (ch=='=') {
						words.push_back(Message(lineNumber,buffer,"$SELF_BIT_AND(��λ����������)","&="));
						state=0,buffer="";
					} else {
						words.push_back(Message(lineNumber,toString(i+1),"$BIT_AND(��λ�������)","&"));
						state=0,buffer="",--i;
					}
					break;
				case 13:
					if (ch=='=') {
						CString value,symbol=buffer;
						if (buffer=="*")
							value="$SELF_MULTI(�Գ������)";
						else if (buffer=="/")
							value="$SELF_DIV(�Գ������)";
						else if (buffer=="%")
							value="$SELF_MOD(��ģ�����)";
						else if (buffer=="^")
							value="$SELF_BIT_XOR(���ֻ������)";
						buffer.Format("%d",i+1);
						words.push_back(Message(lineNumber,buffer,value,symbol+"="));
						state=0,buffer="";
					} else {
						CString value;
						if (buffer=="*")
							value="$MULTIPLY(�˷������)";
						else if (buffer=="/")
							value="$DIVIDE(���������)";
						else if (buffer=="%")
							value="$MODULAR(ģ�������)";
						else if (buffer=="^")
							value="$BIT_XOR(��λ��������)";
						words.push_back(Message(lineNumber,toString(i+1),value,buffer));
						state=0,buffer="",--i;
					}
					break;
				case 14:
					if (isLetter(ch,'F'))
						state=43;
					else if (isLetter(ch,'L'))
						state=15;
					else if (isLetter(ch,'U'))
						state=46;
					else if (!isIdPart(ch)) {
						CString longValue;
						longValue.Format("%ld",atol(buffer));
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_LONG(�����ͳ���)",longValue));
						state=0,buffer="",--i;
					} else {
						CString str;
						str.Format("�����ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 15:
					if (isLetter(ch,'U'))
						state=47;
					else if (!isIdPart(ch)) {
						CString llongValue=buffer;
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_LONGLONG(�������ͳ���)",llongValue));
						state=0,buffer="",--i;
					} else {
						CString str;
						str.Format("�������ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 16:
					if (isLetter(ch,'F'))
						state=43;
					else {
						CString str;
						str.Format("������%s�����������ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 17:
					if (ch=='\\') {
						buffer.AppendChar(ch);
						state=18;
					} else if (ch=='\'') {
						CString str;
						str.Format("�������ڲ�û���ַ���");
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					} else {
						buffer.AppendChar(ch);
						state=19;
					}
					break;
				case 18:
					buffer.AppendChar(ch);
					if (ch>='0' && ch<='3')
						state=20;
					else if (ch=='x')
						state=21;
					else
						state=19;
					break;
				case 19:
					if (ch=='\'') {
						buffer.AppendChar(ch);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_CHAR(���ַ�����)",buffer));
						state=0,buffer="";
					} else {
						CString str;
						str.Format("�������ڲ�����ֻ��һ���ַ��������ַ�'%c'��",ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 20:
					if (ch>='0' && ch<='7') {
						buffer.AppendChar(ch);
						state=22;
					} else if (ch=='\'') {
						buffer.AppendChar(ch);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_CHAR(���ַ�����)",buffer));
						state=0,buffer="";
					} else {
						CString str;
						str.Format("�������ڲ�����ֻ��һ���ַ��������ַ�'%c'��",ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 21:
					if (isHexChar(ch)) {
						buffer.AppendChar(ch);
						state=23;
					} else {
						CString str;
						str.Format("ʮ�����Ʊ�����ڷǷ��ַ�'%c'��",ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 22:
					if (ch>='0' && ch<='7') {
						buffer.AppendChar(ch);
						state=24;
					} else if (ch=='\'') {
						buffer.AppendChar(ch);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_CHAR(���ַ�����)",buffer));
						state=0,buffer="";
					} else {
						CString str;
						str.Format("�������ڲ�����ֻ��һ���ַ��������ַ�'%c'��",ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 23:
					if (isHexChar(ch)) {
						buffer.AppendChar(ch);
						state=19;
					} else if (ch=='\'') {
						buffer.AppendChar(ch);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_CHAR(���ַ�����)",buffer));
						state=0,buffer="";
					} else {
						CString str;
						str.Format("�������ڲ�����ֻ��һ���ַ��������ַ�'%c'��",ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 24:
					if (ch=='\'') {
						buffer.AppendChar(ch);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_CHAR(���ַ�����)",buffer));
						state=0,buffer="";
					} else {
						CString str;
						str.Format("�������ڲ�����ֻ��һ���ַ��������ַ�'%c'��",ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 25:
					if (ch=='=') {
						CString type,symbol=buffer+"=";
						if (buffer=="=")
							type="$IS_EQUAL(�е������)";
						else
							type="$IS_NOT_EQUAL(�в��������)";
						buffer.Format("%d",i+1);
						words.push_back(Message(lineNumber,buffer,type,symbol));
						state=0,buffer="";
					} else {
						CString type,symbol=buffer;
						if (buffer=="=")
							type="$EVALUATE(��ֵ�����)";
						else
							type="$LOGIC_NOT(�߼��������)";
						buffer.Format("%d",i+1);
						words.push_back(Message(lineNumber,toString(i+1),type,symbol));
						state=0,buffer="",--i;
					}
					break;
				case 26:
					buffer.Format("%d",i+1);
					if (ch=='=') {
						words.push_back(Message(lineNumber,buffer,"$IS_LRGEQU(���ڵ��������)",">="));
						state=0,buffer="";
					} else if (ch=='>') {
						state=27;
					} else {
						words.push_back(Message(lineNumber,toString(i+1),"$IS_LARGER(���������)",">"));
						state=0,buffer="",--i;
					}
					break;
				case 27:
					buffer.Format("%d",i+1);
					if (ch=='=') {
						words.push_back(Message(lineNumber,buffer,"$SELF_BIT_SAR(���������������)",">>="));
						state=0,buffer="";
					} else {
						words.push_back(Message(lineNumber,toString(i+1),"$BIT_SAR(�������������)",">>"));
						state=0,buffer="",--i;
					}
					break;
				case 28:
					buffer.Format("%d",i+1);
					if (ch=='=') {
						words.push_back(Message(lineNumber,buffer,"$IS_SMLEQU(С�ڵ��������)","<="));
						state=0,buffer="";
					} else if (ch=='<') {
						state=29;
					} else {
						words.push_back(Message(lineNumber,toString(i+1),"$IS_SMALLER(С�������)","<"));
						state=0,buffer="",--i;
					}
					break;
				case 29:
					buffer.Format("%d",i+1);
					if (ch=='=') {
						words.push_back(Message(lineNumber,buffer,"$SELF_BIT_SAL(���������������)","<<="));
						state=0,buffer="";
					} else {
						words.push_back(Message(lineNumber,toString(i+1),"$BIT_SAL(�������������)","<<"));
						state=0,buffer="",--i;
					}
					break;
				case 30:
					buffer.AppendChar(ch);
					if (ch=='"') {
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_CHAR_STAR(�ַ�������)",buffer));
						state=0,buffer="";
					} else if (ch=='\\')
						state=33;
					else
						state=30;
					break;
				case 31:
					buffer.AppendChar(ch);
					if (isHexChar(ch))
						state=32;
					else {
						CString str;
						str.Format("�ַ���%s��ʮ�����Ʊ�ʾ�ַ�'%c'�Ƿ���",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 32:
					if (ch=='"') // Ϊ�򻯴��룬������»�������30#״̬������β�߼�
						state=30,--i;
					else {
						buffer.AppendChar(ch);
						if (ch=='\\')
							state=33;
						else
							state=30;
					}
					break;
				case 33:
					buffer.AppendChar(ch);
					if (ch=='x')
						state=31;
					else if (ch>='0' && ch<='3')
						state=34;
					else
						state=30;
					break;
				case 34:
					if (ch=='"') // ������ͬ32#״̬
						state=30,--i;
					else {
						buffer.AppendChar(ch);
						if (ch>='0' || ch<='7')
							state=35;
						else if (ch=='\\')
							state=33;
					}
					break;
				case 35:
					if (ch=='"')
						state=30,--i;
					else {
						buffer.AppendChar(ch);
						if (ch>='0' || ch<='7')
							state=30;
						else if (ch=='\\')
							state=33;
					}
					break;
				case 36:
					if (isLetter(ch,'L'))
						state=14;
					else if (isLetter(ch,'U'))
						state=45;
					else if (isLetter(ch,'F'))
						state=44;
					else {
						if (isdigit(ch)) {
							if (ch<'8') {
								buffer.AppendChar(ch);
								state=37;
							} else {
								CString str;
								str.Format("�˽������ͳ���%s����������'%c'��",buffer,ch);
								error.push_back(Message(lineNumber,toString(i+1),err(str)));
								state=-1;
							}
						} else {
							if (ch=='.') {
								buffer.AppendChar(ch);
								state=2;
							} else if (isLetter(ch,'E')) {
								buffer.AppendChar(ch);
								state=3;
							} else if (isLetter(ch,'X')) {
								buffer.AppendChar(ch);
								state=6;
							} else if (!isIdStarter(ch)) {
								buffer.AppendChar(ch);
								CString intValue;
								intValue.Format("%d",atoi(buffer));
								words.push_back(Message(lineNumber,toString(i+1),"$CONST_INT(���ͳ���)",intValue));
								state=0,buffer="",--i;
							} else {
								CString str;
								str.Format("���ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
								error.push_back(Message(lineNumber,toString(i+1),err(str)));
								state=-1;
							}
						}
					}
					break;
				case 37:
					if (ch>='0' && ch<='7') {
						buffer.AppendChar(ch);
						state=37;
					} else if (isLetter(ch,'U')) {
						state=58;
					} else if (isLetter(ch,'L')) {
						state=41;
					} else if (ch>='8' && ch<='9') {
						CString str;
						str.Format("�˽������ͳ���%s����������'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					} else if (!isIdStarter(ch)) {
						CString intValue;
						int value=0;
						for (int j=1;j<buffer.GetLength();j++)
							value=(value<<3)+buffer[j]-'0';
						intValue.Format("%d",value);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_INT(���ͳ���)",intValue));
						state=0,buffer="",--i;
					} else {
						CString str;
						str.Format("���ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 38:
					if (isHexChar(ch)) {
						buffer.AppendChar(ch);
						state=38;
					} else if (isLetter(ch,'U')) {
						state=63;
					} else if (isLetter(ch,'L')) {
						state=39;
					} else if (!isIdStarter(ch)) {
						CString intValue;
						int value;
						sscanf_s(buffer,"%x",&value);
						intValue.Format("%d",value);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_INT(���ͳ���)",intValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("���ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 39:
					if (isLetter(ch,'U')) {
						state=61;
					} else if (isLetter(ch,'L')) {
						state=40;
					} else if (!isIdPart(ch)) {
						CString longValue;
						long value;
						sscanf_s(buffer,"%x",&value);
						longValue.Format("%ld",value);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_LONG(�����ͳ���)",longValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�����ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 40:
					if (isLetter(ch,'U')) {
						state=62;
					} else if (!isIdPart(ch)) {
						CString llongValue;
						long long llong=0;
						for (int j=2;j<buffer.GetLength();j++) {
							if (isdigit(buffer[j]))
								llong=(llong<<4)+buffer[j]-'0';
							else if (isupper(buffer[j]))
								llong=(llong<<4)+buffer[j]-'A'+10;
							else
								llong=(llong<<4)+buffer[j]-'a'+10;
						}
						llongValue.Format("%lld",llong);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_LONGLONG(�������ͳ���)",llongValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�������ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 41:
					if (isLetter(ch,'U')) {
						state=60;
					} else if (isLetter(ch,'L')) {
						state=42;
					} else if (!isIdPart(ch)) {
						CString longValue;
						long value=0;
						for (int j=1;j<buffer.GetLength();j++)
							value=(value<<3)+buffer[j]-'0';
						longValue.Format("%ld",value);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_LONG(�����ͳ���)",longValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�����ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 42:
					if (isLetter(ch,'U')) {
						state=59;
					} else if (!isIdPart(ch)) {
						CString llongValue;
						long long value=0;
						for (int j=1;j<buffer.GetLength();j++)
							value=(value<<3)+buffer[j]-'0';
						llongValue.Format("%lld",value);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_LONGLONG(�������ͳ���)",llongValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�������ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 43:
					if (!isIdPart(ch)) {
						CString doubleValue;
						doubleValue.Format("%g",atof(buffer));
						if (doubleValue.Find(".")==-1)
							doubleValue+=".0";
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_DOUBLE(˫���ȸ��㳣��)",doubleValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("˫���ȸ�����%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 44:
					if (!isIdPart(ch)) {
						CString floatValue;
						floatValue.Format("%g",(float)atof(buffer));
						if (floatValue.Find(".")==-1)
							floatValue+=".0";
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_FLOAT(�����ȸ��㳣��)",floatValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�����ȸ�����%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 45:
					if (!isIdPart(ch)) {
						CString uintValue;
						uintValue.Format("%u",(unsigned)atoi(buffer));
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_UINT(�޷������ͳ���)",uintValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�޷������ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 46:
					if (!isIdPart(ch)) {
						CString ulongValue;
						ulongValue.Format("%lu",(unsigned long)atol(buffer));
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_ULONG(�޷��ų����ͳ���)",ulongValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�޷��ų����ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 47:
					if (!isIdPart(ch)) {
						CString ullongValue;
						unsigned long long ullong;
						sscanf_s(buffer,"%llu",&ullong);
						ullongValue.Format("%llu",ullong);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_ULONGLONG(�޷��ų������ͳ���)",ullongValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�޷��ų������ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 58:
					if (!isIdPart(ch)) {
						CString uintValue;
						int uint=0;
						for (int j=1;j<buffer.GetLength();j++)
							uint=(uint<<3)+buffer[j]-'0';
						uintValue.Format("%u",uint);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_UINT(�޷������ͳ���)",uintValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�޷������ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 59:
					if (!isIdPart(ch)) {
						CString ullongValue;
						unsigned long long ullong=0;
						for (int j=1;j<buffer.GetLength();j++)
							ullong=(ullong<<3)+buffer[j]-'0';
						ullongValue.Format("%llu",ullong);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_ULONGLONG(�޷��ų������ͳ���)",ullongValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�޷��ų������ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 60:
					if (!isIdPart(ch)) {
						CString ulongValue;
						unsigned long ulong=0;
						for (int j=1;j<buffer.GetLength();j++)
							ulong=(ulong<<3)+buffer[j]-'0';
						ulongValue.Format("%lu",ulong);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_ULONG(�޷��ų����ͳ���)",ulongValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�޷��ų����ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 61:
					if (!isIdPart(ch)) {
						CString ulongValue;
						unsigned long ulong;
						sscanf_s(buffer,"%x",&ulong);
						ulongValue.Format("%lu",ulong);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_UINT(�޷������ͳ���)",ulongValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�޷��ų����ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 62:
					if (!isIdPart(ch)) {
						CString ullongValue;
						unsigned long long ullong=0;
						for (int j=2;j<buffer.GetLength();j++) {
							if (isdigit(buffer[j]))
								ullong=(ullong<<4)+buffer[j]-'0';
							else if (isupper(buffer[j]))
								ullong=(ullong<<4)+buffer[j]-'A'+10;
							else
								ullong=(ullong<<4)+buffer[j]-'a'+10;
						}
						ullongValue.Format("%llu",ullong);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_ULONGLONG(�޷��ų������ͳ���)",ullongValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�޷��ų������ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
				case 63:
					if (!isIdPart(ch)) {
						CString uintValue;
						unsigned int uint;
						sscanf_s(buffer,"%x",&uint);
						uintValue.Format("%u",uint);
						words.push_back(Message(lineNumber,toString(i+1),"$CONST_UINT(�޷������ͳ���)",uintValue));
						buffer="", state=0,--i;
					} else {
						CString str;
						str.Format("�޷������ͳ���%s����ڷǷ��ַ�'%c'��",buffer,ch);
						error.push_back(Message(lineNumber,toString(i+1),err(str)));
						state=-1;
					}
					break;
			}
		}
		if (state>0) { // һ��δ���ڽ���״̬
			CString str;
			str.Format("���д���δ��ᣡ",buffer,ch);
			error.push_back(Message(lineNumber,toString(line.GetLength()),err(str)));
		}

		prev=ind+1;
	}

	((CRichEditCtrl*)pParent)->HideSelection(false,false);
	if (ctrl!=NULL) {
		vector<Message>::iterator it;
		for (it=error.begin();it!=error.end();++it) {
			int nRow = ctrl->InsertItem(INT_MAX, it->line); //������
			ctrl->SetItemText(nRow, 1, it->column); //��������
			ctrl->SetItemText(nRow, 2, it->type); //��������
			ctrl->SetItemText(nRow, 3, it->value); //��������
		}
		if (error.size()) {
			int nRow = ctrl->InsertItem(INT_MAX, "***"); //������
			ctrl->SetItemText(nRow, 1, "***"); //��������
			ctrl->SetItemText(nRow, 2, "***"); //��������
			ctrl->SetItemText(nRow, 3, "***"); //��������
		}

		for (it=words.begin();it!=words.end();++it) {
			int nRow = ctrl->InsertItem(INT_MAX, it->line); //������
			ctrl->SetItemText(nRow, 1, it->column); //��������
			ctrl->SetItemText(nRow, 2, it->type); //��������
			ctrl->SetItemText(nRow, 3, it->value); //��������
		}
	}
}
