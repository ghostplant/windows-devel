#include <windows.h>
#include <gl/glut.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int length;
FILE *fp;
float maxMatch;

void RenderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	rewind(fp);
	fscanf(fp,"%d",&length);
	float x,y,diff;
	static char name[MAX_PATH];

	glLineWidth(3);
	for (unsigned i=1;i<=length;i++) {
		fscanf(fp,"%s %f",name,&diff);
		x = i*2.0/(length+1) - 1;
		if (fabs(maxMatch-diff)<1e-8)
			glColor3f(1,0,0);
		else
			glColor3f(1,1,1);
		glBegin(GL_LINES);
		y = -0.8f;
		glVertex2f(x, y);
		y = 1.6f/diff-0.8f;
		glVertex2f(x, y);
		glEnd();
		y=-0.9f;
		glRasterPos2f(x,y);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, (i-1)+'A');

	}

	glutSwapBuffers();
}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpCmd,int nShow) {
	fp=fopen("matchdiff.bin","r");
	if (!fp) {
		MessageBox(0,"没有找到要加载的音频序列！","载入错误",MB_ICONEXCLAMATION);
		return 1;
	}
	fscanf(fp,"%d",&length);
	float diff;
	static char name[MAX_PATH];
	maxMatch=1e20;
	for (unsigned i=1;i<=length;i++) {
		fscanf(fp,"%s %f",name,&diff);
		maxMatch=min(maxMatch,diff);
	}

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000,600);

	glutCreateWindow("OPENGL 可视化演示 - 音频检索结果");
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	fclose(fp);
	return 0;
}
