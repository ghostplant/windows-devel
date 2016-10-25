#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <Windows.h>

int delay=100;
float maxEnergy, maxHalfFreq = 1024;
FILE *fp;

const int tone[12]={261, 277, 293, 311, 329, 349, 370, 392, 415, 440, 466, 493};
const char rep[12]={'C', '#', 'D', '#', 'E', 'F', '#', 'G', '#', 'A', '#', 'B'};

struct Pair {
	float freq,energy;
} pair;

void TimerFunc(int value) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int size;
	fread(&size,4,1,fp);
	if (feof(fp)) {
		fclose(fp);
		exit(0);
	}

	for (int j=1;j<=8;j<<=1) {
		glColor3f((j&1)^1,((j>>1)&1)^1,((j>>2)&1)^1);
		for (int i=0;i<12;i++) {
			float x=(tone[i]*j*0.5/maxHalfFreq)-1,y=-0.95;
			glRasterPos2f(x,y);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, rep[i]);
		}
	}

	glBegin(GL_LINES);
	while (size--) {
		fread(&pair,8,1,fp);

		glColor3f(pair.freq>=256,pair.freq<256||pair.freq>=512,pair.freq<512 || pair.freq>=1024);

		pair.freq=(pair.freq/maxHalfFreq)-1;
		pair.energy=(pair.energy*2/maxEnergy);

		glVertex2f(pair.freq, -0.9);
		glVertex2f(pair.freq, pair.energy-0.9);
	}
	glEnd();
	glFlush();

	glutTimerFunc(delay, TimerFunc, value);
}


void specialKeysListener(int key, int x, int y) {
	if(key == GLUT_KEY_LEFT)
		delay+=25;
	if(key == GLUT_KEY_RIGHT)
		delay=max(25,delay-25);
}

void myDisplay() {
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	fp=fopen("winfft.bin","rb");
	if (!fp)
		return 0;
	int size;
	while (fread(&size,4,1,fp), !feof(fp)) {
		while (size--) {
			fread(&pair,8,1,fp);
			maxEnergy=max(maxEnergy,pair.energy);
		}
	}
	rewind(fp);

	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("OPENGL 可视化演示 - 加窗傅里叶动态频谱");
	glutSpecialFunc(specialKeysListener);
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(delay, TimerFunc, 0);
	glutMainLoop();
	fclose(fp);
	return 0;
}