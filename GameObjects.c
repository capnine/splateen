#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "GameObjects.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



////////////////////////
/*       Camera       */
////////////////////////


void initCamera(Camera *camera,Player *player){
	int i;
	double x[3];
	camera->distance = 10;
	for (i=0; i<3; i++) {
		x[i] = player->position.x[i];
	}
	x[1] -= 5;
	x[2] += 2;
	setVector(&camera->position, x);
	
	setVector(&camera->pointLookedAt, player->position.x);
	
	x[0]=0.0;
	x[1]=0.0;
	x[2]=1.0;
	
	setVector(&camera->head, x);
	
}

void moveCamera(Camera *camera,Player *player){
	double x[3] = {5,0,2};
	Vector *vectorToCameraFromPlayer;
	vectorToCameraFromPlayer = (Vector *)malloc(sizeof(Vector));
	
	setVector(vectorToCameraFromPlayer, x);
	vectorToCameraFromPlayer->x[2] -= 5*sin(player->lookAngleZ / 60);
	rotateVectorInXY(vectorToCameraFromPlayer, player->lookAngleXY+180);
	
	setVector(&camera->position, player->position.x);
	addVector(&camera->position, vectorToCameraFromPlayer);
	setVector(&camera->pointLookedAt, player->position.x);
	camera->pointLookedAt.x[2] += 1.5;
	
	free(vectorToCameraFromPlayer);
}

void lookByCamera(Camera *camera){
	double *posi = camera->position.x;
	double *point = camera->pointLookedAt.x;
	double *head = camera->head.x;
	//カメラ位置[3]、注視点[3]、頭の位置[3]
	gluLookAt(posi[0],posi[1],posi[2],point[0],point[1],point[2],head[0],head[1],head[2]);
	
}





////////////////////////
/*       Player       */
////////////////////////


void initPlayer(Player *player){
	double x1[3]={0,0,0.5};
	double x2[3]={0,0,0};
	double x3[3]={0,0,-PHYSICS_G};
	player->height = 1.0;
	player->radius = 0.3;
	player->state = 0;
	setVector(&player->position, x1);
	setVector(&player->velocity, x2);
	setVector(&player->acceleration, x3);
	player->lookAngleXY = 90;
	player->lookAngleZ = -10;
	player->pauseCount = 0;
}


void setPlayerVelocity(Player *player){
	addVector(&player->velocity, &player->acceleration);
}

void movePlayer(Player *player,Stage *stage,ActionFlag *af){
	int i;
	char collision_flag=0;
	double zero[3] = {0,0,0};
	double velocity;
	Vector *nowPosition;//衝突判定後からの復元用
	Vector *motionVector;//入力による移動ベクトル
	nowPosition = (Vector *)malloc(sizeof(Vector));
	motionVector = (Vector *)malloc(sizeof(Vector));
	
	//入力による移動
	setVector(nowPosition, player->position.x);//復元用バッファ
	setVector(motionVector, zero);
	
	switch (player->state) {
		case 0:
		case 1:
			velocity = 0.1;
			break;
		default:
			velocity = 0;
			break;
	}
	
	if (af->move_up) {
		motionVector->x[0] += velocity;
	}
	if (af->move_down) {
		motionVector->x[0] -= velocity;
	}
	if (af->move_right) {
		motionVector->x[1] -= velocity;
	}
	if (af->move_left) {
		motionVector->x[1] += velocity;
	}
	
	rotateVectorInXY(motionVector, player->lookAngleXY);
	addVector(&player->position, motionVector);
	
	for (i=0; i<stage->numberOfCuboid; i++) {
		if(collidionWithCuboid(&stage->cuboids[i], player)){
			collision_flag ++;
//			printf("%d\n",i);
			break;
		}
	}
	
	if (collision_flag) {
		setVector(&player->position, nowPosition->x);
	}
	
//	printf("\nbefore jump\n");
//	printPlayer(player);
	
	//自然物理による移動(落下)
	collision_flag = 0;
	setVector(nowPosition, player->position.x);
	if (af->jump && (player->state == 0) && (player->pauseCount == 0) &&0) {
		player->state = 1;
		player->velocity.x[2] += 1.0;
		player->pauseCount += 10;
//		printf("%f",player->velocity.x[2]);
	}else{
		if ((player->state == 0) && (player->pauseCount > 0)) {
			player->pauseCount --;
		}
	}
	setPlayerVelocity(player);
	addVector(&player->position, &player->velocity);
	for (i=0; i<stage->numberOfCuboid; i++) {
		if(collidionWithCuboid(&stage->cuboids[i], player)){
			collision_flag ++;
			break;
		}
	}
	if (collision_flag) {
		player->state = 0;
		setVector(&player->velocity, zero);
		setVector(&player->position, nowPosition->x);
	}
	
	free(nowPosition);
	free(motionVector);
	
//	printf("\nafter jump\n");
//	printPlayer(player);
}

void movePlayerLookAngle(Player *player,ActionFlag *af){
	
	if (af->look_right) {
		player->lookAngleXY -= 3;
	}
	if (af->look_left) {
		player->lookAngleXY += 3;
	}
	if (player->lookAngleXY > 360 || player->lookAngleXY < -360) {
		player->lookAngleXY = 0;
	}
	
	if (af->look_up) {
		if (player->lookAngleZ < 100) {
			player->lookAngleZ += 1;
		}
	}
	if (af->look_down) {
		if (player->lookAngleZ > -60) {
			player->lookAngleZ -= 1;
		}
	}
}

void shotBullet(Player *player,BulletList *bulletList){
	Bullet* bullet;
	bullet = (Bullet *)malloc(sizeof(Bullet));
	initBulletWithPlayer(bullet, player);
	addBullet(bulletList, bullet);
}

void drawPlayer(Player *player){
	int i;
	int SLICE = 20;
	double *posi;
	double h = player->height;
	double r = player->radius;
	double theta;
	
	glPushMatrix();
	posi = player->position.x;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colors[GRAY]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colors[BLACK]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colors[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	
	glTranslated(posi[0], posi[1], posi[2] + r);
	glutSolidSphere(r, SLICE, SLICE);
	
	glBegin(GL_QUAD_STRIP);
	for(i=0;i<=SLICE;i++){
		theta = 2*PI *i/SLICE;
		glNormal3f(cos(theta),sin(theta),0.0);
		glVertex3f((r*cos(theta)),r*sin(theta),h);
		glVertex3f((r*cos(theta)),r*sin(theta),0);
	}
	glEnd();
	
	glTranslated(0,0,h);
	glutSolidSphere(r, SLICE, SLICE);
	
	
	glPopMatrix();
}


int collidionWithCuboid(Cuboid *cuboid,Player *player){
	int i;
	int flag1=0;
	int flag2=0;
	int flag3=0;
	int flag4=0;
	double buf[3];
	double player_x = player->position.x[0];
	double player_y = player->position.x[1];
	double player_r = player->radius;
	double player_maxZ = player->position.x[2] + player->height + 2*player_r;
	double player_minZ = player->position.x[2];
	double cube_maxX = cuboid->maxPosition.x[0];
	double cube_minX = cuboid->position.x[0];
	double cube_maxY = cuboid->maxPosition.x[1];
	double cube_minY = cuboid->position.x[1];
	double cube_maxZ = cuboid->maxPosition.x[2];
	double cube_minZ = cuboid->position.x[2];
	Vector *vector1;
	Vector *vector2;
	
	vector1 = (Vector *)malloc(sizeof(Vector));
	vector2 = (Vector *)malloc(sizeof(Vector));
	
	if (((cube_maxZ > player_minZ) && (cube_minZ < player_minZ))||
		((cube_maxZ > player_maxZ) && (cube_minZ < player_maxZ))) {
		flag1 = 1;
//		printf("1:%f 2:%f 3:%f 4:%f\n",cube_maxZ,cube_minZ,player_maxZ,player_minZ);
	}
	buf[0]=player_x;
	buf[1]=player_y;
	buf[2]=0.0;
	setVector(vector1, buf);
	for (i=0; i<4; i++) {
		buf[0]=cuboid->node[i][0]+cuboid->position.x[0];
		buf[1]=cuboid->node[i][1]+cuboid->position.x[1];
		buf[2]=0.0;
		setVector(vector2, buf);
//		printf("1:%f 2:%f 3:%f 4:%f\n",cuboid->node[i][0],cuboid->node[i][1],cuboid->position.x[0],cuboid->position.x[1]);
		if (distanceBetweenVectors(vector1, vector2) < (player_r + MARGIN)) {
			flag2 ++;
		}
	}
	
	if (((cube_maxX + player_r + MARGIN) >  player_x )&&
		((cube_minX - player_r - MARGIN) < player_x )&&
		((cube_maxY + MARGIN) >  player_y )&&
		((cube_minY - MARGIN) < player_y )){
		flag3 ++;
	}
	if (((cube_maxX + MARGIN) >  player_x )&&
		((cube_minX - MARGIN) < player_x )&&
		((cube_maxY + player_r + MARGIN) >  player_y)&&
		((cube_minY - player_r - MARGIN) < player_y)){
		flag4 ++;
	}
	
	free(vector1);
	free(vector2);
	
//	printf("1:%d 2:%d 3:%d 4:%d\n",flag1,flag2,flag3,flag4);
	return flag1 && (flag2 || flag3 || flag4);
}

void printPlayer(Player *player){
	printf("---Player---\n");
	printf("state:%d\n",player->state);
	printf("position:");
	printVector(&player->position);
	printf("velocity:");
	printVector(&player->velocity);
	printf("acceleration:");
	printVector(&player->acceleration);
	printf("------------\n");
}


////////////////////////
/*       Bullet       */
////////////////////////

void initBulletWithPlayer(Bullet *bullet,Player *player){
	Vector *initPosition,*nextPosition;
	Vector *bulletVelocity;
	initPosition = (Vector *)malloc(sizeof(Vector));
	nextPosition = (Vector *)malloc(sizeof(Vector));
	bulletVelocity = (Vector *)malloc(sizeof(Vector));
	double x[3],e[3];
	x[0] = 0.0;
	x[1] = -0.3;
	x[2] = 0.5;
	e[0] = 1.0;
	e[1] = 0.0;
	e[2] = 0.05;
	setVector(initPosition, x);
	rotateVectorInXY(initPosition, player->lookAngleXY);
	addVector(initPosition, &player->position);
	copyVector(&bullet->position, initPosition);
	
	setVector(bulletVelocity, e);
	rotateVectorInXY(bulletVelocity, player->lookAngleXY);
	changeLengthOfVector(bulletVelocity, BULLET_V);
	copyVector(&bullet->velocity, bulletVelocity);
	
	copyVector(nextPosition, initPosition);
	addVector(nextPosition, bulletVelocity);
	copyVector(&bullet->nextPosition, nextPosition);
	
	bullet->radius = BULLET_RADIUS;
	
	free(initPosition);
	free(nextPosition);
	free(bulletVelocity);
}

void moveBullet(Bullet *bullet){
	copyVector(&bullet->position, &bullet->nextPosition);
	bullet->velocity.x[2] -= BULLET_G;
	addVector(&bullet->nextPosition, &bullet->velocity);
}

void moveBullets(BulletList *bulletList){
	BulletListElement *bulletx;
	bulletx = bulletList->firstBulletElement;
	if (bulletx == NULL) {
		return;
	}
	while (bulletx != NULL) {
		moveBullet(bulletx->bullet);
		bulletx = bulletx->next;
	}
}

void drawBullet(Bullet *bullet){
	double *posi;
	double r,SLICE;
	
	r = bullet->radius;
	posi = bullet->position.x;
	SLICE = 20;
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colors[ORANGE]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colors[ORANGE]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colors[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	
	glTranslated(posi[0], posi[1], posi[2] + r);
	glutSolidSphere(r, SLICE, SLICE);
	
	glPopMatrix();
}

void initBulletList(BulletList *bulletList){
	bulletList->counter = 0;
	bulletList = (BulletList *)malloc(sizeof(bulletList));
	bulletList->firstBulletElement = NULL;
	bulletList->lastBulletElement = NULL;
}

void addBullet(BulletList *bulletList,Bullet *bullet){
	BulletListElement *newElement = &bulletList->bulletListElements[bulletList->counter];
	bulletList->counter ++;
	bulletList->counter %= MAX_BULLET;
	if (bulletList->firstBulletElement == NULL) {
		newElement->prev = NULL;
		newElement->next = NULL;
		newElement->bullet = bullet;
		bulletList->firstBulletElement = newElement;
		bulletList->lastBulletElement = newElement;
	}else{
		bulletList->lastBulletElement->next = newElement;
		newElement->prev = bulletList->lastBulletElement;
		newElement->bullet = bullet;
		newElement->next = NULL;
		bulletList->lastBulletElement = newElement;
	}
	return;
}

void drawBullets(BulletList *bulletList){
	BulletListElement *bulletx;
	bulletx = bulletList->firstBulletElement;
	if (bulletx == NULL) {
		return;
	}
	while (bulletx != NULL) {
		drawBullet(bulletx->bullet);
		bulletx = bulletx->next;
	}
//	free(bulletx);
}


////////////////////////
/*       Stage        */
////////////////////////


void initStage(Stage *stage){
	int i;
	Cuboid* cuboids = stage->cuboids;
	stage->numberOfCuboid = 10;
	for(i=0;i<10;i++) initCuboid(&cuboids[i]);
	setCuboidPosition(&cuboids[0],-5,-5,-10);
	setCuboidSize(&cuboids[0],10,10,10);
	setCuboidPosition(&cuboids[1],0,3,0);
	setCuboidSize(&cuboids[1],1,1,3);
	setCuboidPosition(&cuboids[2],3,0,0);
	setCuboidSize(&cuboids[2],1,1,3);
	setCuboidPosition(&cuboids[3],3,3,0);
	setCuboidSize(&cuboids[3],1,1,3);
	setCuboidPosition(&cuboids[4],5,0,0);
	setCuboidSize(&cuboids[4],1,1,3);
	setCuboidPosition(&cuboids[5],5,3,0);
	setCuboidSize(&cuboids[5],1,1,3);
	setCuboidPosition(&cuboids[6],7,0,0);
	setCuboidSize(&cuboids[6],1,1,3);
	setCuboidPosition(&cuboids[7],7,3,0);
	setCuboidSize(&cuboids[7],1,1,3);
	setCuboidPosition(&cuboids[8],9,0,0);
	setCuboidSize(&cuboids[8],1,1,3);
	setCuboidPosition(&cuboids[9],9,3,0);
	setCuboidSize(&cuboids[9],1,1,3);
}

void drawStage(Stage *stage){
	int i;
	Cuboid* cuboids = stage->cuboids;
	for (i=0; i<stage->numberOfCuboid; i++) {
		drawCuboid(&cuboids[i]);
		drawCuboidPaintableFace(&cuboids[i]);
	}
//	free(cuboids);
}

////////////////////////
/*     ActionFlag     */
////////////////////////

void initActionFlag(ActionFlag *af){
	af->move_up = 0;
	af->move_down = 0;
	af->move_left = 0;
	af->move_right = 0;
	af->look_up = 0;
	af->look_down = 0;
	af->look_left = 0;
	af->look_right = 0;
	af->jump = 0;
}

void getActionFlag(ActionFlag *af,int mySpecialValue, int myKeyboardValue){
	af->move_up		= myKeyboardValue & (1 << 1);
	af->move_left	= myKeyboardValue & (1 << 2);
	af->move_right	= myKeyboardValue & (1 << 3);
	af->move_down	= myKeyboardValue & (1 << 4);
	af->look_up		= mySpecialValue & (1 << 0);
	af->look_left	= mySpecialValue & (1 << 1);
	af->look_right	= mySpecialValue & (1 << 2);
	af->look_down	= mySpecialValue & (1 << 3);
	af->jump	= myKeyboardValue & (1 << 0);
}









