#include <windows.h>
#include <gl/glut.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float angle=0.0f,level=1.0f;

int length;
float *tDat;

void RenderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float x=-1,y=0;
	glBegin(GL_LINES);
	for (unsigned i=0;i<length;i++) {
		glVertex2f(x, y);
		x = i*2.0/length - 1;
		y = tDat[i]*level;
		glVertex2f(x, y);
	}
	glEnd();

	glutSwapBuffers();
}

void specialKeysListener(int key, int x, int y) {
	if(key == GLUT_KEY_LEFT)
		angle -= 0.1f;
	if(key == GLUT_KEY_RIGHT)
		angle += 0.1f;
	if(key == GLUT_KEY_UP)
		level += 0.1f;
	if(key == GLUT_KEY_DOWN)
		level -= 0.1f;
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

	glutCreateWindow("OPENGL 可视化演示 - 音频波形图");
	glutSpecialFunc(specialKeysListener);
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	delete []tDat;
	return 0;
}
