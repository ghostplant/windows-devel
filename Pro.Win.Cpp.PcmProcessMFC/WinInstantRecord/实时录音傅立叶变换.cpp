#include <windows.h>
#include <gl/gl.h>
#include <math.h>

const char *CLASSNAME = "OpenGl App";
HWND hWnd;

const int BUFSIZE=4096; // 越小实时性越好，但性能消耗大
const int BUFNUM=128; // BUFSIZE越小，就得要求BUFNUM越大
const int SAMPLENUM=BUFSIZE>>1;
const float DOUBLE_PI = 6.283185307179586476925286766559f;

namespace FFT {

	void fft2b(float xreal [], int p = 11) {
		static float ximag[SAMPLENUM], wreal[SAMPLENUM], wimag[SAMPLENUM], treal, timag, ureal, uimag, arg;
		int n = (1 << p), m, k, i, j, t, a, b, index1, index2;

		for (i = 0; i < n; i++) {
			ximag[i] = 0, a = i, b = 0;
			for (j = 0; j < p; j++) {
				b = (b << 1) + (a & 1);
				a >>= 1;
			}
			if (b > i)
				arg = xreal[i], xreal[i] = xreal[b], xreal[b] = arg;
		}

		arg = -DOUBLE_PI / n;
		treal = cos(arg), timag = sin(arg);
		wreal [0] = 1.0, wimag [0] = 0.0;
		for (j = 1; j + j < n; j++) {
			wreal [j] = wreal [j - 1] * treal - wimag [j - 1] * timag;
			wimag [j] = wreal [j - 1] * timag + wimag [j - 1] * treal;
		}

		for (m = 2; m <= n; m <<= 1)
			for (k = 0; k < n; k += m)
				for (j = 0; j + j < m; j++) {
					index1 = k + j, index2 = index1 + (m >> 1);
					t = n * j / m;
					treal = wreal [t] * xreal [index2] - wimag [t] * ximag [index2];
					timag = wreal [t] * ximag [index2] + wimag [t] * xreal [index2];
					ureal = xreal [index1];
					uimag = ximag [index1];
					xreal [index1] = ureal + treal;
					ximag [index1] = uimag + timag;
					xreal [index2] = ureal - treal;
					ximag [index2] = uimag - timag;
				}
		for (i = 0; i < n; i++)
			xreal[i] = xreal[i] * xreal[i] + ximag[i] * ximag[i];
	}
}

namespace CAudio {

	HWAVEIN m_hWaveIn;
	HWAVEOUT m_hWaveOut;
	WAVEFORMATEX sndfmt;
	WAVEHDR* m_bufferIn[BUFNUM];
	WAVEHDR* m_bufferOut[BUFNUM]; int top;

	void PushRecord(WAVEHDR *pWaveInBuffer) {
		waveInPrepareHeader(m_hWaveIn,pWaveInBuffer,sizeof(WAVEHDR));
		waveInAddBuffer(m_hWaveIn,pWaveInBuffer,sizeof(WAVEHDR));
	}
	
	void PopRecord(WAVEHDR *pWaveInBuffer) {
		waveInUnprepareHeader(m_hWaveIn,pWaveInBuffer, sizeof(WAVEHDR));
	}
	
	void PushPlay(WAVEHDR *pWaveInBuffer) {
		WAVEHDR* one_buffer=m_bufferOut[--top];
		memcpy(one_buffer,pWaveInBuffer,sizeof(WAVEHDR)+pWaveInBuffer->dwBufferLength);
		waveOutPrepareHeader(m_hWaveOut,one_buffer,sizeof(WAVEHDR));
		waveOutWrite(m_hWaveOut,one_buffer,sizeof(WAVEHDR));
	}
	
	void PopPlay(WAVEHDR *pWaveOutBuffer) {
		waveOutUnprepareHeader(m_hWaveOut,pWaveOutBuffer,sizeof(WAVEHDR));
	}

	bool BeginRecord(HWND hWnd) {
		if (!waveInGetNumDevs() || !waveOutGetNumDevs())
			return false;
		sndfmt.wFormatTag=WAVE_FORMAT_PCM;
		sndfmt.nChannels=1;
		sndfmt.wBitsPerSample=16;
		sndfmt.nSamplesPerSec=44100;
		sndfmt.nAvgBytesPerSec=(sndfmt.wBitsPerSample>>3)*sndfmt.nSamplesPerSec;
		sndfmt.nBlockAlign=(sndfmt.wBitsPerSample>>3)*sndfmt.nChannels;
		sndfmt.cbSize=0;
		
		if (waveInOpen(&m_hWaveIn,WAVE_MAPPER,&sndfmt,(DWORD)hWnd,0,CALLBACK_WINDOW)!=MMSYSERR_NOERROR)
			return false;
		if (waveOutOpen(&m_hWaveOut,WAVE_MAPPER,&sndfmt,(DWORD)hWnd,0,CALLBACK_WINDOW)!=MMSYSERR_NOERROR)
			return false;
		for (int i=0;i<BUFNUM;i++) {
			m_bufferIn[i]=(WAVEHDR*)malloc(BUFSIZE+sizeof(WAVEHDR));
			ZeroMemory(m_bufferIn[i],sizeof(WAVEHDR));
			m_bufferIn[i]->lpData=(LPSTR)(m_bufferIn[i]+1);
			m_bufferIn[i]->dwBufferLength=BUFSIZE;
			PushRecord(m_bufferIn[i]);
		}
		for (top=0;top<BUFNUM;top++) {
			m_bufferOut[top]=(WAVEHDR*)malloc(BUFSIZE+sizeof(WAVEHDR));
			ZeroMemory(m_bufferOut[top],sizeof(WAVEHDR));
			m_bufferOut[top]->dwBufferLength = BUFSIZE;
			m_bufferOut[top]->lpData = (LPSTR)(m_bufferIn[top] +1);
		}
		return waveOutReset(m_hWaveOut)==MMSYSERR_NOERROR && waveInStart(m_hWaveIn)==MMSYSERR_NOERROR;
	}

	bool EndRecord() {
		return waveInStop(m_hWaveIn)==MMSYSERR_NOERROR && waveInReset(m_hWaveIn)==MMSYSERR_NOERROR
			&& waveInClose(m_hWaveIn)==MMSYSERR_NOERROR && waveOutClose(m_hWaveOut)==MMSYSERR_NOERROR;
	}

	void DrawSample(LPSTR lpData) {
		glBegin(GL_LINE_STRIP);
		short* samples = (short*)lpData;
		glVertex2f(-1.0f, 0.0f);
		for (int i=0;i<SAMPLENUM;i++)
			glVertex2f(i*2.0/SAMPLENUM-1, samples[i]/32768.0f);
		glVertex2f(1.0f, 0.0f);
		glEnd();
	}

	void DrawZerocross(LPSTR lpData) {
		glBegin(GL_LINE_STRIP);
		short* samples = (short *)lpData;
		int cnt=0;
		for (int i=1;i<SAMPLENUM;i++) {
			if (samples[i-1]*samples[i]<0)
				++cnt;
		}
		glVertex2f(-1.0f, cnt*2.0/SAMPLENUM-1);
		glVertex2f(1.0f, cnt*2.0/SAMPLENUM-1);
		glEnd();
	}

	void DrawFrequency(LPSTR lpData) {
		glBegin(GL_LINE_STRIP);
		short* samples = (short *)lpData;
		float standard[BUFSIZE];
		int cnt=0;
		for (int i=0;i<SAMPLENUM;i++)
			standard[i]=samples[i]/32768.0f;
		FFT::fft2b(standard);
		glVertex2f(-1.0f, -0.8f);
		const int PREFIX = 200;
		/*int best=0;
		for (int i=1;i<PREFIX;i++)
			if (standard[i]>standard[best])
				best=i;
		glVertex2f((best-1)*2.0/PREFIX-1, -0.8f);
		glVertex2f(best*2.0/PREFIX-1, standard[best]/16.0f - 0.8f);
		glVertex2f((best+1)*2.0/PREFIX-1, -0.8f);*/

		for (int i=0;i<PREFIX;i++)
			glVertex2f(i*2.0/PREFIX-1, standard[i]/16.0f - 0.8f);
		glVertex2f(1.0f, -0.8f);
		glEnd();
	}

	void DrawRecord(LPSTR lpData) {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//DrawSample(lpData);
		//DrawZerocross(lpData);
		DrawFrequency(lpData);

		SwapBuffers(GetDC(hWnd));
	}

	void GetInBufferFull(WAVEHDR* lp) {
		PopRecord(lp);

		DrawRecord(lp->lpData);
		//PushPlay(lp);

		PushRecord(lp);
	}

	void PutOutBufferEmpty(WAVEHDR* lp) {
		PopPlay(lp);
		m_bufferOut[top++]=lp;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_SHOWWINDOW:
			CAudio::BeginRecord(hWnd);
			break;
		case WM_CLOSE:
			//CAudio::EndRecord();
			PostQuitMessage(0);
			break;
		case MM_WIM_DATA:
			CAudio::GetInBufferFull((WAVEHDR*)lParam);
			break;
		case MM_WOM_DONE:
			CAudio::PutOutBufferEmpty((WAVEHDR*)lParam);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HGLRC EnableOpenGL(HWND hWnd) {
	PIXELFORMATDESCRIPTOR pfd = {0};
	pfd.nSize = sizeof(pfd);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24, pfd.cDepthBits = 16;
	HDC hDC = GetDC(hWnd);
	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	HGLRC hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	return hRC;
}

void DisableOpenGL(HWND hWnd, HGLRC hRC) {
	wglMakeCurrent(0, 0);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, GetDC(hWnd));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow) {
	WNDCLASS wc = {0};
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.hbrBackground = (HBRUSH)7;
	wc.lpszClassName = CLASSNAME;
	RegisterClass(&wc);

	hWnd = CreateWindow(CLASSNAME, "OpenGl 应用程序", WS_VISIBLE | WS_CAPTION | WS_SYSMENU, 50, 50, 500, 500, 0, 0, hInstance, 0);
	HGLRC hRC = EnableOpenGL(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
		TranslateMessage(&msg), DispatchMessage(&msg);

	DisableOpenGL(hWnd, hRC);
	return msg.wParam;
}
