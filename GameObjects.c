#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "GameObjects.h"
#include <stdio.h>



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
	x[1] -= 10;
	x[2] += 2;
	setVector(&camera->position, x);
	
	setVector(&camera->pointLookedAt, player->position.x);
	
	x[0]=0.0;
	x[1]=0.0;
	x[2]=1.0;
	
	setVector(&camera->head, x);
	
}

void moveCamera(Camera *camera,Player *player){
	double x[3] = {10,0,2};
	Vector vectorToCameraFromPlayer;
	
	setVector(&vectorToCameraFromPlayer, x);
	rotateVectorInXY(&vectorToCameraFromPlayer, player->lookAngleXY+180);
	
	setVector(&camera->position, player->position.x);
	addVector(&camera->position, &vectorToCameraFromPlayer);
	setVector(&camera->pointLookedAt, player->position.x);
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
}


void setPlayerVelocity(Player *player){
	addVector(&player->velocity, &player->acceleration);
}

void movePlayer(Player *player,Stage *stage,ActionFlag *af){
	int i;
	char collision_flag=0;
	double zero[3] = {0,0,0};
	double velocity;
	Vector nowPosition;//衝突判定後からの復元用
	Vector motionVector;//入力による移動ベクトル
	
	//入力による移動
	setVector(&nowPosition, player->position.x);//復元用バッファ
	setVector(&motionVector, zero);
	
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
		motionVector.x[0] += velocity;
	}
	if (af->move_down) {
		motionVector.x[0] -= velocity;
	}
	if (af->move_right) {
		motionVector.x[1] -= velocity;
	}
	if (af->move_left) {
		motionVector.x[1] += velocity;
	}
	
	rotateVectorInXY(&motionVector, player->lookAngleXY);
	addVector(&player->position, &motionVector);
	
	for (i=0; i<stage->numberOfCuboid; i++) {
		if(collidionWithCuboid(&stage->cuboids[i], player)){
			collision_flag ++;
//			printf("%d\n",i);
			break;
		}
	}
	
	if (collision_flag) {
		setVector(&player->position, nowPosition.x);
	}
	
//	printf("\nbefore jump\n");
//	printPlayer(player);
	
	//自然物理による移動(落下)
	collision_flag = 0;
	setVector(&nowPosition, player->position.x);
	if (af->jump && (player->state == 0)) {
		player->state = 1;
		player->velocity.x[2] += 1.0;
//		printf("%f",player->velocity.x[2]);
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
		setVector(&player->position, nowPosition.x);
	}
	
//	printf("\nafter jump\n");
//	printPlayer(player);
}

void movePlayerLookAngle(Player *player,ActionFlag *af){
	double angle	= player->lookAngleXY;
	
	if (af->look_right) {
		player->lookAngleXY -= 1;
	}
	if (af->look_left) {
		player->lookAngleXY += 1;
	}
	
	if (angle > 360 || angle < -360) {
		angle = 0;
	}
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
	
	if (((cube_maxZ > player_minZ) && (cube_minZ < player_minZ))||
		((cube_maxZ > player_maxZ) && (cube_minZ < player_maxZ))) {
		flag1 = 1;
//		printf("1:%f 2:%f 3:%f 4:%f\n",cube_maxZ,cube_minZ,player_maxZ,player_minZ);
	}
	Vector vector1;
	Vector vector2;
	setVectorWithXYZ(&vector1, player_x, player_y, 0.0);
	for (i=0; i<4; i++) {
		setVectorWithXYZ(&vector2, cuboid->node[i][0]+cuboid->position.x[0], cuboid->node[i][1]+cuboid->position.x[1],0.0);
//		printf("1:%f 2:%f 3:%f 4:%f\n",cuboid->node[i][0],cuboid->node[i][1],cuboid->position.x[0],cuboid->position.x[1]);
		if (distanceBetweenVectors(&vector1, &vector2) < (player_r + MARGIN)) {
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
	}
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









