#include <windows.h>
#include <gl/glut.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int length,flip,lcnt,accum;
unsigned char *tone,*trend;
unsigned int *last;
float *freq;
int minTone,maxTone, toneRange;
float minFreq,maxFreq,freqRange;

void RenderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float x,y;
	x=-1, y=0.5;
	glColor3f(1,1,1);
	glBegin(GL_LINES);
	accum=0;
	for (unsigned i=0;i<lcnt;i++) {
		if (tone[i]==255)
			continue;
		for (int j=last[i];j;j--) {
			glVertex2f(x, y);
			x = accum*2.0/flip - 1;
			y = float(tone[i]-minTone)/toneRange;
			++accum;
			glVertex2f(x, y);
		}
	}
	x=-1, y=0.5;
	glColor3f(0,0,1);
	glBegin(GL_LINES);
	accum=0;
	for (unsigned i=0;i<lcnt;i++) {
		if (tone[i]==255)
			continue;
		for (int j=last[i];j;j--) {
			glVertex2f(x, y);
			x = accum*2.0/flip - 1;
			y = float(trend[i]-minTone)/toneRange+0.01;
			++accum;
			glVertex2f(x, y);
		}
	}

	x=-1,y=-0.5;
	for (unsigned i=0;i<length;i++) {
		if (freq[i]>2148 || freq[i]<100)
			continue;
		glVertex2f(x, y);
		x = i*2.0/length - 1;
		y = (freq[i]-maxFreq)/freqRange;
		glVertex2f(x, y);
	}
	glEnd();

	glutSwapBuffers();
}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpCmd,int nShow) {
	FILE *fp=fopen("toneseq.bin","rb"),*fs=fopen("freqseq.bin","rb");
	if (!fp || !fs) {
		MessageBox(0,"没有找到要加载的音频序列！","载入错误",MB_ICONEXCLAMATION);
		return 1;
	}
	fread(&lcnt,4,1,fp);
	tone=new unsigned char[lcnt];
	trend=new unsigned char[lcnt];
	last=new unsigned int[lcnt];
	fread(tone,1,lcnt,fp);
	fread(last,4,lcnt,fp);
	fclose(fp);

	flip=0;
	for (int i=0;i<lcnt;i++) {
		flip+=last[i];
	}

	fread(&length,4,1,fs);
	freq=new float[length];
	fread(freq,4,length,fs);
	fclose(fs);

	minTone=255, maxTone=0;
	for (unsigned i=0;i<lcnt;i++) {
		if (tone[i]==255)
			continue;
		minTone=min(minTone,tone[i]);
		maxTone=max(maxTone,tone[i]);
	}
	toneRange=maxTone-minTone+1;

	minFreq=2148,maxFreq=100;
	for (unsigned i=0;i<length;i++) {
		if (freq[i]>2148 || freq[i]<100)
			continue;
		minFreq=min(minFreq,freq[i]);
		maxFreq=max(maxFreq,freq[i]);
	}
	freqRange=maxFreq-minFreq+1;

	////////////////////消除毛刺////////////////////
	if (tone[0]==255)
		tone[0]=0;
	for (int j=1;j<lcnt;j++) {
		if (tone[j]==255)
			tone[j]=tone[j-1];
	}

	memcpy(trend,tone,lcnt);
	for (int j=0,i=1;i<lcnt;i++) {
		if (abs(trend[i]-trend[j])>1)
			j = i;
		else
			trend[i] = trend[j];
	}
	for (int i=1;i<lcnt;i++) {
		if (last[i]<4)
			trend[i]=trend[i-1];
	}
	/* FILE *fout;
	fout=fopen("D:\\out.txt","w");
	for (int i=0;i<lcnt;i++)
		fprintf(fout,"%d(%d..)\n",tone[i],last[i]);
	fclose(fout);*/
	////////////////////////////////////////////////

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000,600);

	glutCreateWindow("OPENGL 可视化演示 - 频率变化(下)与基因确定(上)");
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	delete []tone;
	return 0;
}
