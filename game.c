#include <stdio.h>
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

int GameOverCount = 0;

Vector *firstPlayerPosition;
Vector *firstCompPosition;

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
	
	//PLAYER
	getActionFlag(af, mySpecialValue, myKeyboardValue);
	movePlayer(player1, mainStage,af);
	moveCamera(mainCamera, player1);
	moveBullets(bulletList,mainStage);
	if (collisionPlayerWithBullets(player1, bulletList)
		|| (player1->position.x[2]< -STAGE_MAX_Z)) {
		killPlayer(player1, firstPlayerPosition);
	}
	if (af->shot  && (player1->shotPauseCount == 0)) {
		shotBullet(player1, bulletList);
		player1->shotPauseCount = PLAYER_SHOT_INTERVAL;
	}else{
		if(player1->shotPauseCount>0)player1->shotPauseCount --;
	}
	movePlayerLookAngle(player1, af);
	
	//COMP
	getCompAciton(afOfCom);
	movePlayer(comPlayer, mainStage,afOfCom);
	if (collisionPlayerWithBullets(comPlayer, bulletList)
		|| (comPlayer->position.x[2]< -STAGE_MAX_Z)) {
		killPlayer(comPlayer, firstCompPosition);
	}
	if (afOfCom->shot  && (comPlayer->shotPauseCount == 0)) {
		shotBullet(comPlayer, bulletList);
		comPlayer->shotPauseCount = PLAYER_SHOT_INTERVAL;
	}else{
		if(comPlayer->shotPauseCount>0)comPlayer->shotPauseCount --;
	}

	glLoadIdentity();
	
//	if (af->look_up) {
//		printf("Score is %d\n",getScore(mainStage, 1));
//	}
	
	lookByCamera(mainCamera);
	
	if (GameOverCount++ < MAX_GAME_TIME) {
		if (GameOverCount%100 == 0) {
			printf("%d\n",(MAX_GAME_TIME-GameOverCount)/100);
		}
		glutTimerFunc(10, myTimerFunc, 0);
	}else{
		printf("*********************\n");
		printf("******GAME OVER******\n");
		printf("*********************\n");
		
		printf("- Your score is %d -\n",getScore(mainStage, 1));
	}
}

void init(void){
	
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
	firstPlayerPosition = (Vector *)malloc(sizeof(Vector));
	firstCompPosition = (Vector *)malloc(sizeof(Vector));
	
	initController();
	initActionFlag(af);
	initActionFlag(afOfCom);
	initPlayer(player1);
	player1->color = PLAYER_COLOR;
	initCamera(mainCamera, player1);
	initVectorWithXYZ(firstPlayerPosition, 0, 0, 0.1);
	
	initPlayer(comPlayer);
	comPlayer->isComp = 1;
	comPlayer->color = COMP_COLOR;
	comPlayer->lookAngleXY = -90;
	initVectorWithXYZ(firstCompPosition, 0, 50, 0.1);
	setPlayerPosition(comPlayer, firstCompPosition);
	
	
	initBulletList(bulletList);
	initStage(mainStage);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1, .1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, -10.0, 2.0, 0.0, 0.0, 1.5, 0.0, 0.0, 1.0);
	glLightfv(GL_LIGHT1, GL_POSITION, pos0);
	glLightfv(GL_LIGHT1, GL_POSITION, pos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, colors[WHITE]);
	myTimerFunc(0);
}
