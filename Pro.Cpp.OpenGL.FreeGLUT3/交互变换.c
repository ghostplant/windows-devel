/*
实验：图形学实验四
学号：0911080114
班级：09065501
姓名：崔嵬
*/

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define GL_PI 3.1415f

static GLfloat xRot = 0.0f, yRot = 0.0f;
static GLfloat xPos = 0.0f, yPos = 0.0f, zPos = 0.0f;
static GLfloat scale = 1.0f;

void displayScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 透视效果
	gluPerspective(90.0f, 1.0f, 3.0f, 60.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 15.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);

	// 设置光线
	{
		GLfloat light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat light_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

		glPushMatrix();
			glRotatef(yRot, 0.0f, 1.0f, 0.0f);
			glRotatef(xRot, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, 75.0f);

			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glPopMatrix();

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}

	// 设置锥体
	{
		GLfloat cone_ambient[]  = {0.0f, 0.0f, 0.5f, 1.0f};
		GLfloat cone_diffuse[]  = {0.0f, 0.0f, 0.5f, 1.0f};
		GLfloat cone_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};
		GLfloat cone_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat cone_shininess  = 30.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT,   cone_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   cone_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  cone_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION,  cone_emission);
		glMaterialf (GL_FRONT, GL_SHININESS, cone_shininess);

		glPushMatrix();
			glRotatef(xPos, 1.0f, 0.0f, 0.0f);
			glRotatef(yPos, 0.0f, 1.0f, 0.0f);
			glRotatef(zPos, 0.0f, 0.0f, 1.0f);
			glScalef(scale, scale, scale);
			glTranslatef(3.0f, 0.0f, 0.0f);
			glutSolidCone(2.0f, 6.0f, 16, 16);
		glPopMatrix();
	}

	glutSwapBuffers();
}

void specialKeysListener(int key, int x, int y) {
	if(key == GLUT_KEY_UP)
		xRot-= 5.0f;
	if(key == GLUT_KEY_DOWN)
		xRot += 5.0f;
	if(key == GLUT_KEY_LEFT)
		yRot -= 5.0f;
	if(key == GLUT_KEY_RIGHT)
		yRot += 5.0f;
	if(key > 356.0f)
		xRot = 0.0f;
	if(key < -1.0f)
		xRot = 355.0f;
	if(key > 356.0f)
		yRot = 0.0f;
	if(key < -1.0f)
		yRot = 355.0f;

	glutPostRedisplay();
}

void mouseListener(int button,int state,int x, int y) {
	switch (button) {
		case GLUT_LEFT_BUTTON:
			scale*=0.9;
			break;
		case GLUT_RIGHT_BUTTON:
			scale/=0.9;
			break;
	}
	glutPostRedisplay();
}

void keyboardListener(unsigned char key,int x, int y) {
	switch (key) {
		case 'w':
			yPos+=5;
			break;
		case 's':
			yPos-=5;
			break;
		case 'a':
			xPos+=5;
			break;
		case 'd':
			xPos-=5;
			break;
		case 'q':
			zPos+=5;
			break;
		case 'e':
			zPos-=5;
			break;
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	printf("ASDF-QE：控制锥体\n\n鼠标：控制椎体大小\n\n上下：左右控制光源\n");
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Interaction Transform");
	glutSpecialFunc(specialKeysListener);
	glutDisplayFunc(displayScene);
	glutMouseFunc(mouseListener);
	glutKeyboardFunc(keyboardListener);
	glutMainLoop();
	return 0;
}
