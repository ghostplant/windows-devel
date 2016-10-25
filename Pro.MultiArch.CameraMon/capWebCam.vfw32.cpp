#include <afxwin.h>
#include "bmpjpeg.h"

#include <vfw.h>
#pragma comment(lib, "vfw32.lib")

void showMemoryDIB(HDC dc,char *bits) {
	HDRAWDIB hDrawDib=DrawDibOpen();
	DrawDibDraw(hDrawDib,dc,0,0,256,192,(BITMAPINFOHEADER*)bits,
		bits+sizeof(BITMAPINFOHEADER),0,0,((int*)bits)[1],((int*)bits)[2],0);
	DrawDibClose(hDrawDib);
}

HWND capInitialize(int freq = 25) {
	HWND hWebCam=capCreateCaptureWindow("摄像头监控", WS_VISIBLE|WS_CAPTION|WS_THICKFRAME|WS_SYSMENU, 256, 0, 640, 480, NULL, 0);
	capDriverConnect(hWebCam,0);
	capPreview(hWebCam,TRUE);
	capPreviewRate(hWebCam,freq);
	return hWebCam;
}

void capToMemoryDIB(HWND hWebCam, char *outdata,int *dSize) {
	capEditCopy(hWebCam);
	OpenClipboard(NULL);
	HANDLE hModule=GetClipboardData(CF_DIB);
	BYTE *bits=(BYTE*)GlobalLock(hModule);
	int height=((int*)bits)[1], width=((int*)bits)[2];

	*dSize=40+height*width*3;
	memcpy(outdata,bits,*dSize);
	CloseClipboard();
}

void scrToMemoryDIB(char* outdata,int *dSize) {
    HWND window = GetDesktopWindow();
    HDC _dc = GetWindowDC(window);//屏幕DC
    HDC dc = CreateCompatibleDC(0);//内存DC
    
    RECT re;
    GetWindowRect(window, &re);
    int w = re.right,
        h = re.bottom;
    void* buf = new char[w*h*4];
    
    HBITMAP bm = CreateCompatibleBitmap(_dc, w, h);//建立和屏幕兼容的bitmap
    SelectObject(dc, bm);//将memBitmap选入内存DC    
    StretchBlt(dc, 0, 0, w, h, _dc, 0, 0, w, h, SRCCOPY);//复制屏幕图像到内存DC
    
    GetObject(bm, 84, buf);
    
    tagBITMAPINFO bi;
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = 0;
    bi.bmiHeader.biSizeImage = 0;
    
    CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &buf, 0, 0);
    GetDIBits(dc, bm, 0, h, buf, &bi, DIB_RGB_COLORS);

    BITMAPINFOHEADER bii;
    bii.biSize = 40;
    bii.biWidth = w;
    bii.biHeight = h;
    bii.biPlanes = 1;
    bii.biBitCount = 32;
    bii.biCompression = 0;
    bii.biSizeImage = (w*h)<<2;
    
    memcpy(outdata,&bii,sizeof(bii));
    memcpy(outdata+sizeof(bii),buf,bii.biSizeImage);
    *dSize=sizeof(bii)+bii.biSizeImage;

    DeleteDC(_dc);
    DeleteDC(dc);
}

class CMainFrame:public CFrameWnd {
	HWND hWebCam;
public:

	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus) {
		hWebCam=capInitialize();
		SetTimer(0,100,0);
	}

	afx_msg void OnTimer(UINT nEvent) {
		static char buf[1<<22];
		int size;
		capToMemoryDIB(hWebCam,buf,&size);
		//scrToMemoryDIB(buf,&size);
		showMemoryDIB(::GetDC(m_hWnd),buf);
	}

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMainFrame,CFrameWnd)
ON_WM_TIMER()
ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

class CMyApp:public CWinApp {
public:
	BOOL InitInstance() {
		CMainFrame *frame=new CMainFrame();
		frame->Create(0,"Windows App");
		frame->ShowWindow(TRUE);
		m_pMainWnd=frame;
		return TRUE;
	}
} theApp;
