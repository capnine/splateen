#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "head.h"
#include "material.h"
#include "physics.h"

GLfloat pos0[] = { 5.0, 5.0, 5.0, 1.0 };
GLfloat pos1[] = { 5.0, 5.0, 5.0, 1.0 };
GLfloat color[][4] = {
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, 0.0, 0.0, 1.0 },
		{ 0.0, 1.0, 0.0, 1.0 },
		{ 0.0, 0.0, 1.0, 1.0 },
		{ 1.0, 1.0, 0.0, 1.0 },
		{ 1.0, 0.0, 1.0, 1.0 },
		{ 0.0, 1.0, 1.0, 1.0 },
		{ 0.7, 0.7, 0.7, 1.0 },
		{ 0.0, 0.0, 0.0, 1.0 } };//色を増やす場合はここに追加

int X = 100;//地面
int Y = 100;
double L = 1;

double x = 0;
double y = 0;
double z = 0;
int mySpecialValue = 0;
double tekiList[][3] = {
		{ 0.0, 2.0, 0.0 },
		{ 4.0, 4.0, 0.0 },
		{ 2.0, 6.0, 0.0 },
		{ 0.0, 10.0, 0.0 },
		{ 4.0, 13.0, 0.0 },
		{ 3.0, 14.0, 0.0 },
		{ 2.0, 15.0, 0.0 },
		{ 1.0, 16.0, 0.0 },
		{ 3.0, 19.0, 0.0 },
		{ 2.0, 20.0, 0.0 },
		{ 1.0, 21.0, 0.0 },
		{ 0.0, 22.0, 0.0 } };//障害物のリスト
int tekiIndex = 12;
double v = 0;
double theta;

void idle(void)
{
	glutPostRedisplay();
}

int collision()
{
	//衝突判定
	int i;
	double MARGIN = 0.05;
	if (z > 1)return 0;
	for (i = 0; i < tekiIndex; i++)
	{
		//簡単な衝突判定
		if ((tekiList[i][0] - x <1 - MARGIN) && (tekiList[i][0] - x >-1 + MARGIN)
			&& (tekiList[i][1] - y <1 - MARGIN) && (tekiList[i][1] - y >-1 + MARGIN))
		{
			printf("(%.02f,%.02f):(%.02f,%.02f)\n", x, y, tekiList[i][0], tekiList[i][1]);
			return 1;
		}
	}
	return 0;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	//drawJiki();
	//drawTeki();
	drawGround();

	glPopMatrix();
	glutSwapBuffers();
}
void myTimerFunc(int value)
{
	double MARGIN = 0.05;
	if (mySpecialValue & (1 << 0))
	{
		y += 0.1;
		if (collision())y -= 0.1;
		if (Y*L < y - MARGIN)y -= 0.1;
	}
	if (mySpecialValue & (1 << 1))
	{
		x -= 0.1;
		if (collision())x += 0.1;
		if (0 * L > x + MARGIN)x += 0.1;
	}
	if (mySpecialValue & (1 << 2))
	{
		x += 0.1;
		if (collision())x -= 0.1;
		if ((X - 1)*L < x - MARGIN)x -= 0.1;
	}
	if (mySpecialValue & (1 << 3))
	{
		y -= 0.1;
		if (collision())y += 0.1;
		if (0 * L > y + MARGIN)y += 0.1;
	}

	if (z > 0)
	{
		v -= 0.01;
		z += v;
		if (collision())
		{
			z -= v;
			v = 0;
		}
		if (z <= 0)
		{
			z = 0;
			v = 0;
		}
	}

	//視点を移動
	glLoadIdentity();
	gluLookAt(0.0 + x, -10.0 + y, 2.0, 0.0 + x, 0.0 + y, 1.5, 0.0, 0.0, 1.0);

	glutTimerFunc(10, myTimerFunc, 0);
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		v = 0.2;
		z += v;
		break;
	}
}
void mySpcialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		mySpecialValue |= 1 << 0;//mySpecialValueの1bit目を1にする
		break;
	case GLUT_KEY_LEFT:
		mySpecialValue |= 1 << 1;//mySpecialValueの2bit目を1にする
		break;
	case GLUT_KEY_RIGHT:
		mySpecialValue |= 1 << 2;//mySpecialValueの3bit目を1にする
		break;
	case GLUT_KEY_DOWN:
		mySpecialValue |= 1 << 3;//mySpecialValueの4bit目を1にする
		break;
	default:
		break;
	}
}
void mySpcialUpFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		mySpecialValue &= ~(1 << 0);//mySpecialValueの1bit目を0にする
		break;
	case GLUT_KEY_LEFT:
		mySpecialValue &= ~(1 << 1);//mySpecialValueの2bit目を0にする
		break;
	case GLUT_KEY_RIGHT:
		mySpecialValue &= ~(1 << 2);//mySpecialValueの3bit目を0にする
		break;
	case GLUT_KEY_DOWN:
		mySpecialValue &= ~(1 << 3);//mySpecialValueの4bit目を0にする
		break;
	default:
		break;
	}

}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutKeyboardFunc(myKeyboardFunc);
	glutSpecialFunc(mySpcialFunc);
	glutSpecialUpFunc(mySpcialUpFunc);

	glutIdleFunc(idle);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1, .1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, -10.0, 2.0, 0.0, 0.0, 1.5, 0.0, 0.0, 1.0);
	glLightfv(GL_LIGHT1, GL_POSITION, pos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color[WHITE]);
	myTimerFunc(0);
}
