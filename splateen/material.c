#include <GLUT/glut.h>
#include <math.h>
#include "physics.h"
#include "material.h"


GLfloat color[][4] = {
	{ 1.0, 1.0, 1.0, 1.0 },
	{ 1.0, 0.0, 0.0, 1.0 },
	{ 0.0, 1.0, 0.0, 1.0 },
	{ 0.0, 0.0, 1.0, 1.0 },
	{ 1.0, 1.0, 0.0, 1.0 },
	{ 1.0, 0.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0, 1.0 },
	{ 0.7, 0.7, 0.7, 1.0 },
	{ 0.0, 0.0, 0.0, 1.0 } };//êFÇëùÇ‚Ç∑èÍçáÇÕÇ±Ç±Ç…í«â¡


//////////////////////////
/*       Cuboid         */
//////////////////////////


int normalCuboidFace[6][4]={
	{0,1,2,3},
	{4,5,6,7},
	{0,1,5,4},
	{1,2,6,5},
	{2,3,7,6},
	{3,0,4,7}
};

void setCuboidNode(Cuboid *cuboid){
	int i,j;
	//íºï˚ëÃÇÃÇWí∏ì_Çê›íËÇ∑ÇÈäÓñ{ÉxÉNÉgÉã
	int vector[8][3]={
		{0,0,0},
		{1,0,0},
		{1,1,0},
		{0,1,0},
		{0,0,1},
		{1,0,1},
		{1,1,1},
		{0,1,1}
	};
	
	for(i=0;i<8;i++){
		for(j=0;j<3;j++){
			cuboid->node[i][j]=cuboid->size3d[j]*vector[i][j] + cuboid->position3d[j];
		}
	}
}

void initCuboid(Cuboid *cuboid){
	int i,j;
	cuboid->position3d[0]=0.0;
	cuboid->position3d[1]=0.0;
	cuboid->position3d[2]=0.0;
	cuboid->size3d[0]=1.0;
	cuboid->size3d[1]=1.0;
	cuboid->size3d[2]=1.0;
	cuboid->color=WHITE;
	setCuboidNode(cuboid);
	
	for(i=0;i<6;i++){
		for(j=0;j<4;j++){
			cuboid->face[i][j]=normalCuboidFace[i][j];
		}
	}
}

void setCuboidSize3d(Cuboid *cuboid,double x,double y,double z){
	cuboid->size3d[0]=x;
	cuboid->size3d[1]=y;
	cuboid->size3d[2]=z;
	setCuboidNode(cuboid);
}

void setCuboidPosition3d(Cuboid *cuboid,double x,double y,double z){
	cuboid->position3d[0]=x;
	cuboid->position3d[1]=y;
	cuboid->position3d[2]=z;
	setCuboidNode(cuboid);
}

void drawCuboid(Cuboid *cuboid){
	int i, j;
	GLdouble *xnode[4];
	GLdouble xnormal[3];//housen vector

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color[GRAY]);//äDêF
	glMaterialfv(GL_FRONT, GL_AMBIENT, color[BLACK]);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, color[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glColor3fv(color[cuboid->color]);
	for (i = 0; i < 6; i++){
		glBegin(GL_QUADS);
		for (j = 0; j < 4; j++){
			xnode[j] = cuboid->node[cuboid->face[i][j]];
			glVertex3dv(xnode[j]);
		}
		calcNormal(xnode[0],xnode[1],xnode[2],xnormal);
		glNormal3dv(xnormal);
		glEnd();
	}

	glPopMatrix();
}


//////////////////////////
/*        Player        */
//////////////////////////

void initPlayer(Player *player){
	player->position3d[0]=0;
	player->position3d[1]=0;
	player->position3d[2]=0;
	player->height=1.2;
	player->radius=0.3;
	player->color=1;
	player->velocity[0]=0;
	player->velocity[1]=0;
	player->velocity[2]=0;
}

void setPlayerPosition(Player *player,double x, double y, double z){
	player->position3d[0] = x;
	player->position3d[1] = y;
	player->position3d[2] = z;
}

void drawPlayer(Player *player){
	glPushMatrix();
	glTranslatef(player->position3d[0], player->position3d[1], player->position3d[2]);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color[GREEN]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, color[BLACK]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, color[WHITE]);
	//glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	
	drawCylinder(player->radius, player->height, 20);
	glPopMatrix();

}


////////////////////////////
/*         Stage          */
////////////////////////////

void initStage(Stage *stage){
	int i;
	Cuboid *cuboids = stage->cuboids;
	for (i=0; i<AMOUNT_OF_CUBOIDS; i++) {
		initCuboid(&cuboids[i]);
	}
	setCuboidSize3d(&cuboids[0], 20, 20, 10);
	setCuboidPosition3d(&cuboids[0],-10,-10,-10);
	setCuboidPosition3d(&cuboids[1],0,3,0);
	setCuboidPosition3d(&cuboids[2],3,0,0);
	setCuboidPosition3d(&cuboids[3],3,3,0);
	setCuboidPosition3d(&cuboids[4],5,0,0);
	setCuboidPosition3d(&cuboids[5],5,3,0);
	setCuboidPosition3d(&cuboids[6],7,0,0);
	setCuboidPosition3d(&cuboids[7],7,3,0);
	setCuboidPosition3d(&cuboids[8],9,0,0);
	setCuboidPosition3d(&cuboids[9],9,3,0);
}

void drawStage(Stage *stage){
	int i;
	for (i=0; i<AMOUNT_OF_CUBOIDS; i++) {
		drawCuboid(&stage->cuboids[i]);
	}
}

////////////////////////
/*        円柱         */
////////////////////////

//radius:半径　height:高さ　sides:円をn多角形で近似
void drawCylinder(float radius,float height,int sides)
{
	int i;
	double theta;
	//上面
	glNormal3d(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	for(i = 0; i < sides; i++) {
		theta = 2*PI * i/sides;
		glVertex3d(radius * cos(theta), radius * sin(theta),height);
	}
	glEnd();
	//下面
	glNormal3d(0.0, 0.0, -1.0);
	glBegin(GL_POLYGON);
	for(i = sides-1 ; i > 0; i--) {
		theta = 2*PI * i/sides;
		glVertex3d(radius * cos(theta), radius * sin(theta),0);
	}
	glEnd();
	//側面
	glBegin(GL_QUAD_STRIP);
	for(i=0;i<=sides;i++){
		theta = 2*PI * i/sides;
		glNormal3f((GLfloat)cos(theta),(GLfloat)sin(theta),0);
		glVertex3f((GLfloat)(radius*cos(theta)),(GLfloat)(radius*sin(theta)),height);
		glVertex3f((GLfloat)(radius*cos(theta)),(GLfloat)(radius*sin(theta)),0);
	}
	glEnd();
}











