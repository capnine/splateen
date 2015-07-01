#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include "head.h"
#include "material.h"
#include "physics.h"
#include "controller.h"

GLfloat pos0[] = { 5.0, 5.0, 5.0, 1.0 };
GLfloat pos1[] = { 10.0, 10.0, 5.0, 1.0 };

int X = 100;//’n–Ê
int Y = 100;
double L = 1;
int mySpecialValue = 0;
int myKeyboardValue = 0;
double v = 0;
double theta;
double cameraPosition[3];
int cameraAngle;

Stage mainstage;
Player player1;

void idle(void)
{
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	drawPlayer(&player1);
	drawStage(&mainstage);

	glPopMatrix();
	glutSwapBuffers();
}

void myTimerFunc(int value)
{
	int i;
	int collision_flag=0;
	double theta;
	double velocity[3] = {0,0,0};
	double nextPosition[3];
	Player nextPlayer;
	player1.velocity[2] -= PHYSICS_G * 0.003;
	nextPosition[0] = player1.position3d[0];
	nextPosition[1] = player1.position3d[1];
	nextPosition[2] = player1.position3d[2];
	if (mySpecialValue & (1 << 0))
	{
		velocity[1] = 0.1;
	}
	if (mySpecialValue & (1 << 1))
	{
		velocity[0] -= 0.1;
	}
	if (mySpecialValue & (1 << 2))
	{
		velocity[0] += 0.1;
	}
	if (mySpecialValue & (1 << 3))
	{
		velocity[1] -= 0.1;
	}
	
	if (myKeyboardValue & (1<<0)) {
		nextPosition[2] += 3;
	}
	if (myKeyboardValue & (1<<2)) {
		cameraAngle += 2;
	}
	if (myKeyboardValue & (1<<3)) {
		cameraAngle -= 2;
	}

	cameraAngle %= 360;
	theta = 2*PI*cameraAngle/360;
	nextPosition[0] += cos(theta) * velocity[0] - sin(theta) * velocity[1];
	nextPosition[1] += sin(theta) * velocity[0] + cos(theta) * velocity[1];
	
	initPlayer(&nextPlayer);
	setPlayerPosition(&nextPlayer, nextPosition[0], nextPosition[1], nextPosition[2]);
	for (i=0; i<10; i++) {
		if (collidionWithCuboid(&mainstage.cuboids[i], &nextPlayer)) {
			collision_flag ++;
			printf("collide with %d\n",i);
			break;
		}
	}
	if (!(collision_flag)) {
		player1.position3d[0] = nextPosition[0];
		player1.position3d[1] = nextPosition[1];
		player1.position3d[2] = nextPosition[2];
	}else{
	}
	
	initPlayer(&nextPlayer);
	nextPosition[0] = player1.position3d[0];
	nextPosition[1] = player1.position3d[1];
	nextPosition[2] = player1.position3d[2];
	nextPosition[2] += player1.velocity[2];
	setPlayerPosition(&nextPlayer, nextPosition[0], nextPosition[1], nextPosition[2]);
	for (i=0; i<10; i++) {
		if (collidionWithCuboid(&mainstage.cuboids[i], &nextPlayer)) {
			collision_flag ++;
			printf("collide with %d\n",i);
			break;
		}
	}
	
	
	if (!(collision_flag)) {
		player1.position3d[0] = nextPosition[0];
		player1.position3d[1] = nextPosition[1];
		player1.position3d[2] = nextPosition[2];
	}else{
		player1.velocity[2]=0;
	}
	
	
	
	
	//Ž‹“_‚ðˆÚ“®
	glLoadIdentity();
	cameraPosition[0] = 10*sin(theta) + player1.position3d[0];
	cameraPosition[1] = -10*cos(theta) + player1.position3d[1];
	cameraPosition[2] = 2.0 + player1.position3d[2];
	gluLookAt(cameraPosition[0],cameraPosition[1], cameraPosition[2],player1.position3d[0],player1.position3d[1],
			  player1.position3d[2] + 2.0, 0.0, 0.0, 1.0);

	glutTimerFunc(10, myTimerFunc, 0);
}


void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glutKeyboardFunc(myKeyboardFunc);
	glutKeyboardUpFunc(myKeyboardUpFunc);
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
	
	initStage(&mainstage);
	initPlayer(&player1);
	setPlayerPosition(&player1, -1, -1, 0.2);
}

