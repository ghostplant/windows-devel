/*
实验：图形学实验三
学号：0911080114
班级：09065501
姓名：崔嵬
*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

GLfloat  whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat  sourceLight[] = { 0.9f, 0.5f, 0.5f, 1.0f };
GLfloat	 lightPos[] = { -600.0f, 0.0f, 0.0f, 1.0f };
GLint	 lightMove = 20;

GLfloat  ispecref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };

void RenderScene() {
	static float fMoonRot = 0.0f;
	static float fEarthRot = 0.0f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

	*lightPos+=lightMove; // 更新光源位置
	if (fabs(*lightPos)>600.0f)
		lightMove=-lightMove;

	glTranslatef(0.0f, 0.0f, -300.0f);
	
	ispecref[0]=ispecref[1]=ispecref[2]=fabs(*lightPos)/300; // 更新材质

	glMaterialfv(GL_FRONT, GL_SPECULAR,ispecref);
	glMateriali(GL_FRONT,GL_SHININESS,64);

	// 恒星
	
	glColor3ub(255, 255, 255);
	glutSolidSphere(50.0f, 50, 140);

	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

	// 行星

	glRotatef(fEarthRot, 0.0f, 0.0f, 1.0f);

	glColor3ub(100,100,255);
	glTranslatef(220.0f,0.0f,0.0f);
	glutSolidSphere(15.0f, 15, 15);

	// 卫星
	glColor3ub(250,250,250);
	glRotatef(fMoonRot,0.0f, 1.0f, 0.0f);
	glTranslatef(20.0f, 0.0f, 0.0f);
	fMoonRot+= 20.0f;

	glutSolidSphere(6.0f, 15, 15);

	glPopMatrix();

	fEarthRot += 5.0f;

	glutSwapBuffers();
}

void SetupRC() {
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight); // 环境光
	glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight); // 散射光
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos); // 光源位置
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
}


void TimerFunc(int value) {
	glutPostRedisplay();
	glutTimerFunc(40, TimerFunc, value);
}

void ChangeSize(int w, int h){
	GLfloat fAspect;

	glViewport(0, 0, w, h);
	fAspect = (GLfloat)w/(GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90.0f, fAspect, 1.0, 425.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Light Automate");

	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(40, TimerFunc, 0);
	SetupRC();
	glutMainLoop();
	return 0;
}
