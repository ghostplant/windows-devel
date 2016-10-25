/*
实验：图形学实验一
学号：0911080114
班级：09065501
姓名：崔嵬
*/

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

void RenderScene() {
	int i;
	const GLfloat RND_MAX=(GLfloat)RAND_MAX, GL_PI=acos(-1.0);
	//对颜色缓存，用设定的背景颜色清屏
	glClear(GL_COLOR_BUFFER_BIT);

	// 画点不同样式的点
	for (i=1;i<10;i++) {
		glPointSize(i+i);
		glBegin(GL_POINTS);
		  glColor3f(rand()/RND_MAX, rand()/RND_MAX, rand()/RND_MAX);
		glVertex2i(i*20,10);
		glEnd();
	}

	// 画线段组样式1
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(20,0xFFFF);
	glLineWidth(10);

	glBegin(GL_LINES);
	  glColor3f(0.0, 1.0, 0.0);						//绿色
	  glVertex2i(180, 15);							//定义顶点坐标
	  glVertex2i(10, 145);							//结束定义一组顶点坐标
  	  glColor3f(0.0, 0.0, 1.0);						//蓝色
	  glVertex2i(10,  15);							//定义顶点坐标
	  glVertex2i(180,145);							//结束定义一组顶点坐标
	glEnd();

	// 画线段组样式2
	glLineStipple(10,0x5555);
	glLineWidth(2);

	glBegin(GL_LINES);
	  glColor3f(1.0, 0.0, 0.0);						//绿色
	  glVertex2i(180, 145);							//定义顶点坐标
	  glVertex2i(10, 145);							//结束定义一组顶点坐标
	glEnd();

	// 画三角形
	glBegin(GL_TRIANGLES);
	  glColor3f(1.0, 1.0, 1.0);						//绿色
	glVertex2i(150.0f, 20.0f);
	glVertex2i(75.0f, 40.0f);
	glVertex2i(25.0f, 20.0f);
	glEnd();

	// 画圆
	/*glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(20.0f, 60.0f);
	float ang;
	for(ang = 0.0f; ang <= (2.0f*GL_PI)+0.5; ang += (GL_PI/16.0f))
	{
		float x = 10.0f*sin(ang), y = 10.0f*cos(ang);
		glVertex2f(20.0f+x, 60.0f+y);
	}
	glEnd();*/

	// 画多边形
	glBegin(GL_POLYGON);
	  glColor3f(0.0f, 0.0f, 1.0f);
	  glVertex2i(199.0f, 99.0f);
	  glVertex2i(179.0f, 99.0f);
	  glColor3f(1.0f, 0.0f, 0.0f);
	  glVertex2i(139.0f, 89.0f);
	  glVertex2i(129.0f, 69.0f);

	  glColor3f(0.0f, 1.0f, 0.0f);
	  glVertex2i(149.0f, 69.0f);
	  glVertex2i(159.0f, 89.0f);
	  glVertex2i(189.0f, 79.0f);
	glEnd();

	glFlush();                     //OpenGL命令常常会排队并成批处理以优化性能，该函数刷新命令队列和缓冲区，
}

int main(int argc,char **argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	//设置显示模式为单缓冲，RGB颜色模式
	glutInitWindowSize(800, 600);

	glutCreateWindow("Basic Elements");

	glClearColor(0.0, 0.0, 0.0, 0.0);				//设定清除颜色缓冲区时的背景颜色为黑色
	glMatrixMode(GL_PROJECTION);					//制定当前操作矩阵为投影矩阵	
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);				//设定当前为二维正投影

	glutDisplayFunc(RenderScene);					//注册显示回调函数，只要重绘窗口，GLUT就会调用该函数
	glutMainLoop();									//显示所有创建的窗口，开始循环，处理鼠标、键盘及窗口消息
	return 0;
}
