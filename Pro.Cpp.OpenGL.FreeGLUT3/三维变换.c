/*
实验：图形学实验二
学号：0911080114
班级：09065501
姓名：崔嵬
*/

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

static GLfloat xRot = 12.0f, yRot = 24.0f, zRot = 36.0f; // 旋转参数
static GLfloat xMov = 0.0f, yMov = 0.0f, zMov = 0.0f; // 平移参数
static GLfloat xSca = 1.0f, ySca = 1.0f, zSca = 1.0f; // 放缩参数

void RenderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

   	glRotatef(xRot, 1.0f, 0.0f, 0.0f); // 旋转
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	glTranslatef(xMov, yMov, zMov); // 平移

	glScalef(xSca, ySca, zSca); // 放缩

	// 前面
	glColor3ub((GLubyte) 255, (GLubyte)255, (GLubyte)0);
	glBegin(GL_POLYGON);
		glVertex3f(20.0f,32.0f,25.0f);
		glVertex3f(20.0f,-32.0f,25.0f);
		glVertex3f(-20.0f,-32.0f,25.0f);
		glVertex3f(-20.0f,32.0f,25.0f);
	glEnd();

	// 后面
	glColor3ub((GLubyte) 255, (GLubyte)0, (GLubyte)255);
	glBegin(GL_POLYGON);
		glVertex3f(20.0f,32.0f,-25.0f);
		glVertex3f(20.0f,-32.0f,-25.0f);
		glVertex3f(-20.0f,-32.0f,-25.0f);
		glVertex3f(-20.0f,32.0f,-25.0f);
	glEnd();

	// 上面
	glColor3ub((GLubyte) 0, (GLubyte)255, (GLubyte)255);
	glBegin(GL_POLYGON);
		glVertex3f(20.0f,32.0f,-25.0f);
		glVertex3f(20.0f,32.0f,25.0f);
		glVertex3f(-20.0f,32.0f,25.0f);
		glVertex3f(-20.0f,32.0f,-25.0f);
	glEnd();

	// 下面
	glColor3ub((GLubyte) 0, (GLubyte)0, (GLubyte)255);
	glBegin(GL_POLYGON);
		glVertex3f(20.0f,-32.0f,-25.0f);
		glVertex3f(20.0f,-32.0f,25.0f);
		glVertex3f(-20.0f,-32.0f,25.0f);
		glVertex3f(-20.0f,-32.0f,-25.0f);
	glEnd();

	// 左面
	glColor3ub((GLubyte) 0, (GLubyte)255, (GLubyte)0);
	glBegin(GL_POLYGON);
		glVertex3f(20.0f,32.0f,25.0f);
		glVertex3f(20.0f,32.0f,-25.0f);
		glVertex3f(20.0f,-32.0f,-25.0f);
		glVertex3f(20.0f,-32.0f,25.0f);
	glEnd();

	// 右面
	glColor3ub((GLubyte) 255, (GLubyte)0, (GLubyte)0);
	glBegin(GL_POLYGON);
		glVertex3f(-20.0f,32.0f,25.0f);
		glVertex3f(-20.0f,32.0f,-25.0f);
		glVertex3f(-20.0f,-32.0f,-25.0f);
		glVertex3f(-20.0f,-32.0f,25.0f);
	glEnd();

	glPopMatrix();
	glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y) {

	switch (key) {
		case 1:
			xSca*= 0.9f;
			break;
		case 2:
			xSca/= 0.9f;
			break;
		case 3:
			ySca*= 0.9f;
			break;
		case 4:
			ySca/= 0.9f;
			break;
		case 5:
			zSca*= 0.9f;
			break;
		case 6:
			zSca/= 0.9f;
			break;
		case 7:
			xMov-= 3.0f;
			break;
		case 8:
			xMov+= 3.0f;
			break;
		case 9:
			yMov-= 3.0f;
			break;
		case 10:
			yMov+= 3.0f;
			break;
		case 11:
			zMov-= 3.0f;
			break;
		case 12:
			zMov+= 3.0f;
			break;
		case GLUT_KEY_UP:
			xRot+= 3.0f;
			break;
		case GLUT_KEY_DOWN:
			xRot-= 3.0f;
			break;
		case GLUT_KEY_LEFT:
			yRot+= 3.0f;
			break;
		case GLUT_KEY_RIGHT:
			zRot+= 3.0f;
			break;
	}

	glutPostRedisplay();
}


void ChangeSize(int w, int h) {
	GLfloat nRange = 100.0f;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h) 
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
	else 
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

	//GLfloat fAspect = (GLfloat)w/(GLfloat)h;
	//gluPerspective(60.0f, fAspect, 100.0, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	printf("按键说明：\n\nF1-F6为缩放，\nF7-F12为平移，\n上下左右为旋转。");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3D Tranform");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f );
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glutMainLoop();

	return 0;
}
