#include <stdlib.h>
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
GLfloat pos1[] = { 0.0, 0.0, 10.0, 1.0 };

Camera *mainCamera;
Player *player1;
Player *comPlayer;
Stage *mainStage;
BulletList *bulletList;
ActionFlag *af;
ActionFlag *afOfCom;

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
	
	drawPlayer(player1);
	drawPlayer(comPlayer);
	drawStage(mainStage);
	drawBullets(bulletList);

	glPopMatrix();
	glutSwapBuffers();
}
void myTimerFunc(int value){
	
	getActionFlag(af, mySpecialValue, myKeyboardValue);
	movePlayer(player1, mainStage,af);
	movePlayer(comPlayer, mainStage,afOfCom);
	moveCamera(mainCamera, player1);
	moveBullets(bulletList,mainStage);
	if (af->shot  && (player1->shotPauseCount == 0)) {
		shotBullet(player1, bulletList);
		player1->shotPauseCount = PLAYER_SHOT_INTERVAL;
	}else{
		if(player1->shotPauseCount>0)player1->shotPauseCount --;
	}
	movePlayerLookAngle(player1, af);

	glLoadIdentity();
	
	lookByCamera(mainCamera);

	glutTimerFunc(10, myTimerFunc, 0);
}

void init(void){
	Vector *buf;
	buf = (Vector *)malloc(sizeof(Vector));
	
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
	
	mainCamera = (Camera *)malloc(sizeof(Camera));
	player1 = (Player *)malloc(sizeof(Player));
	comPlayer = (Player *)malloc(sizeof(Player));
	mainStage = (Stage *)malloc(sizeof(Stage));
	bulletList = (BulletList *)malloc(sizeof(BulletList));
	af = (ActionFlag *)malloc(sizeof(ActionFlag));
	afOfCom = (ActionFlag *)malloc(sizeof(ActionFlag));
	
	initController();
	initActionFlag(af);
	initActionFlag(afOfCom);
	initPlayer(player1);
	player1->color = PLAYER_COLOR;
	
	initPlayer(comPlayer);
	comPlayer->color = COMP_COLOR;
	initVectorWithXYZ(buf, 0, 50, 5);
	setPlayerPosition(comPlayer, buf);
	
	initCamera(mainCamera, player1);
	initBulletList(bulletList);
	initStage(mainStage);

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
