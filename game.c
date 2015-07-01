#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include "game.h"
#include "controller.h"
#include "GameObjects.h"

GLfloat pos0[] = { 5.0, 5.0, 5.0, 1.0 };
GLfloat pos1[] = { 5.0, 5.0, 5.0, 1.0 };

Camera mainCamera;
Player player1;
Stage mainStage;

void idle(void)
{
	glutPostRedisplay();
}

int collision()
{
	
	return 0;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	drawPlayer(&player1);
	drawStage(&mainStage);

	glPopMatrix();
	glutSwapBuffers();
}
void myTimerFunc(int value){
	
	movePlayer(&player1, mySpecialValue);
	moveCamera(&mainCamera, &player1, myKeyboardValue);

	glLoadIdentity();
	
	lookByCamera(&mainCamera);

	glutTimerFunc(10, myTimerFunc, 0);
}

void init(void){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glutKeyboardFunc(myKeyboardFunc);
	glutKeyboardFunc(myKeyboardUpFunc);
	glutSpecialFunc(mySpcialFunc);
	glutSpecialUpFunc(mySpcialUpFunc);

	glutIdleFunc(idle);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	
	initController();
	initPlayer(&player1);
	initCamera(&mainCamera, &player1);
	initStage(&mainStage);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1, .1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, -10.0, 2.0, 0.0, 0.0, 1.5, 0.0, 0.0, 1.0);
	glLightfv(GL_LIGHT1, GL_POSITION, pos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, colors[WHITE]);
	myTimerFunc(0);
}
