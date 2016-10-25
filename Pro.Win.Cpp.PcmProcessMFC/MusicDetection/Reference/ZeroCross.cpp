#include <windows.h>
#include <gl/glut.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float rate=200.0f;

int length;
float *tDat;

void RenderScene() {

	unsigned long WINSZ=1<<11;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double x=-1,y=0;
	glBegin(GL_LINES);
	unsigned ans=0;
	for (unsigned i=1;i<WINSZ;i++) {
		if (tDat[i-1]*tDat[i]<0)
			ans++;
	}
	for (unsigned i=WINSZ;i<length;i++) {
		if (tDat[i-WINSZ]*tDat[i-WINSZ+1]<0)
			ans--;
		if (tDat[i]*tDat[i-1]<0)
			ans++;
		glVertex2f(x, y);
		x = i*2.0/length-1;
		y = ans/rate;
		glVertex2f(x, y);
	}
	glEnd();

	glutSwapBuffers();
}

void specialKeysListener(int key, int x, int y) {
	if(key == GLUT_KEY_UP)
		rate *= 0.9f;
	if(key == GLUT_KEY_DOWN)
		rate /= 0.9f;
	glutPostRedisplay();
}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpCmd,int nShow) {
	FILE *fp=fopen("timeseq.bin","rb");
	if (!fp) {
		MessageBox(0,"没有找到要加载的音频序列！","载入错误",MB_ICONEXCLAMATION);
		return 1;
	}
	fread(&length,4,1,fp);
	tDat=new float[length];
	fread(tDat,4,length,fp);
	fclose(fp);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000,600);

	glutCreateWindow("音频过零率图(上下键调整幅度)");
	glutSpecialFunc(specialKeysListener);
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	delete []tDat;
	return 0;
}
