#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "physics.h"
#include "material.h"

GLfloat colors[][4] = {
	{ 1.0, 1.0, 1.0, 1.0 },
	{ 1.0, 0.0, 0.0, 1.0 },
	{ 0.0, 1.0, 0.0, 1.0 },
	{ 0.0, 0.0, 1.0, 1.0 },
	{ 1.0, 1.0, 0.0, 1.0 },
	{ 1.0, 0.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0, 1.0 },
	{ 0.7, 0.7, 0.7, 1.0 },
	{ 0.0, 0.0, 0.0, 1.0 } };


////////////////////////
/*        Node        */
////////////////////////
void initNode(Node *node, double x[3]){
	setVector(&node->position, x);
}

////////////////////////
/*        Squre       */
////////////////////////

//引数のnodesは面の表から見て反時計回りの順番でなくてはならない
void initSquareWith4Nodes(Square *square,Node nodes[]){
	int i;
	double length;
	Vector buf;
	
	//squareのnodesをセット
	for (i=0; i<4; i++) {
		initNode(&square->nodes[i], nodes[i].position.x);
	}
	
	//squareのゼロ点(基準点)をセット
	copyVector(&square->zeroNode, &nodes[0].position);
	
	//basicVector[0]とsize[0]をセット
	copyVector(&buf, &nodes[1].position);
	minusVector(&buf, &nodes[0].position);
	length = getValueOfVector(&buf);
	changeLengthOfVector(&buf, 1.0/length);
	square->size[0] = length;
	copyVector(&square->basicVector[0], &buf);
	
	//basicVector[1]とsize[1]をセット
	copyVector(&buf, &nodes[3].position);
	minusVector(&buf, &nodes[0].position);
	length = getValueOfVector(&buf);
	changeLengthOfVector(&buf, 1.0/length);
	square->size[1] = length;
	copyVector(&square->basicVector[1], &buf);
	
	//法線ベクトルをセット
	setNormalVector(&square->normalVector, &square->basicVector[0], &square->basicVector[1]);
	
	initPaintSquare(&square->paintSquare, square->size);
}

void initCuboidFace(CuboidFace *cuboidFace,Node nodes[]){
	initSquareWith4Nodes(&cuboidFace->squareFace, nodes);
	cuboidFace->isPaintable = 1;
}

void initPaintSquare(PaintSquare *paintSquare,double size[]){
	int i,j;
	for (i=0; i<2; i++) {
		paintSquare->numberOfElement[i] = (int)size[i] / PAINTCELL_SIZE;
	}
	for (i=0; i<paintSquare->numberOfElement[0]; i++) {
		for (j=0; j<paintSquare->numberOfElement[1]; j++) {
			paintSquare->state[i][j] = 0;
		}
	}
	if ((paintSquare->numberOfElement[0] > 128)||(paintSquare->numberOfElement[1] > 128)) {
		printf("PaintSquareの大きさ制限を超えています。\n");
		return;
	}
}


////////////////////////
/*       Cuboid       */
////////////////////////


int normalFace[6][4]={
	{3,2,1,0},
	{4,5,6,7},
	{0,1,5,4},
	{1,2,6,5},
	{2,3,7,6},
	{3,0,4,7}
};

void setNode(Cuboid *cuboid){
	int i,j;
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
	
	//      7 --- 6
	//     /|    /|
	//    4 +-- 5 |
	//    | 3 --+ 2
	//    |/    |/
	//    0 --- 1    ->x
	
	
	for(i=0;i<8;i++){
		for(j=0;j<3;j++){
			cuboid->node[i][j]=(double)cuboid->size3d[j]*vector[i][j] + cuboid->position.x[j];
		}
		initNode(&cuboid->nodes[i], cuboid->node[i]);
	}
}

void initCuboid(Cuboid *cuboid){
	int i,j;
	cuboid->position.x[0]=0.0;
	cuboid->position.x[1]=0.0;
	cuboid->position.x[2]=0.0;
	cuboid->size3d[0]=1.0;
	cuboid->size3d[1]=1.0;
	cuboid->size3d[2]=1.0;
	cuboid->color=0;
	setNode(cuboid);
	
	for(i=0;i<6;i++){
		for(j=0;j<4;j++){
			cuboid->face[i][j]=normalFace[i][j];
		}
	}
	
}

void setCuboidSize(Cuboid *cuboid,double x,double y,double z){
	cuboid->size3d[0]=x;
	cuboid->size3d[1]=y;
	cuboid->size3d[2]=z;
	setCuboidAllParameter(cuboid);
}

void setCuboidPosition(Cuboid *cuboid,double x,double y,double z){
	double* posi = cuboid->position.x;
	posi[0]=x;
	posi[1]=y;
	posi[2]=z;
	setCuboidAllParameter(cuboid);
}

void setCuboidMaxPosition(Cuboid *cuboid){
	int i;
	for (i=0; i<3; i++) {
		cuboid->maxPosition.x[i] = cuboid->position.x[i] + cuboid->size3d[i];
	}
}

void setCuboidCuboidFace(Cuboid *cuboid){
	//nodesセット実行後が前提
	int i,j;
	Node buf[4];
	for (i=0; i<6; i++) {
		for (j=0; j<4; j++) {
			buf[j] = cuboid->nodes[cuboid->face[i][j]];
		}
		initCuboidFace(&cuboid->paintableFaces[i], buf);
	}
	
}

void setCuboidNormalvec(Cuboid *cuboid){
	//setCuboidCuboidFace後
	int i,j;
	for (i=0; i<6; i++) {
		for (j=0; j<3; j++) {
			cuboid->normalvec[i][j] = cuboid->paintableFaces[i].squareFace.normalVector.x[j];
		}
	}
}

void setCuboidAllParameter(Cuboid *cuboid){
	setCuboidMaxPosition(cuboid);
	setNode(cuboid);
	setCuboidCuboidFace(cuboid);
	setCuboidNormalvec(cuboid);
}

void drawPaintSquare(Square *square){
	int i,j,k,l;
	GLdouble v[4][3];
	Vector offset;
	Vector buf[4];
	Vector basicVector[4];
//	buf = (Vector *)malloc(sizeof(Vector)*4);
//	basicVector = (Vector *)malloc(sizeof(Vector)*4);
	
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colors[BLACK]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colors[BLACK]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colors[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glTranslatef(square->zeroNode.x[0], square->zeroNode.x[1], square->zeroNode.x[2]);
	copyVector(&offset, &square->normalVector);
	changeLengthOfVector(&offset, 0.001);
	glTranslatef(offset.x[0], offset.x[1], offset.x[2]);
	glBegin(GL_QUADS);
	for (i = 0; i < square->paintSquare.numberOfElement[0]; i++){
		for (j = 0; j < square->paintSquare.numberOfElement[1]; j++){
//			if (!((square->paintSquare.state[i][j]) & 1))continue;
			if ((i^j)&1) continue;
			copyVector(&basicVector[0], &square->basicVector[0]);
			copyVector(&basicVector[1], &square->basicVector[0]);
			copyVector(&basicVector[2], &square->basicVector[1]);
			copyVector(&basicVector[3], &square->basicVector[1]);
			changeLengthOfVector(&basicVector[0], i*PAINTCELL_SIZE);
			changeLengthOfVector(&basicVector[1], (i+1)*PAINTCELL_SIZE);
			changeLengthOfVector(&basicVector[2], j*PAINTCELL_SIZE);
			changeLengthOfVector(&basicVector[3], (j+1)*PAINTCELL_SIZE);
			copyVector(&buf[0], &basicVector[0]);
			addVector(&buf[0], &basicVector[2]);
			copyVector(&buf[1], &basicVector[1]);
			addVector(&buf[1], &basicVector[2]);
			copyVector(&buf[2], &basicVector[1]);
			addVector(&buf[2], &basicVector[3]);
			copyVector(&buf[3], &basicVector[0]);
			addVector(&buf[3], &basicVector[3]);
			glVertex3dv(buf[0].x);
			glVertex3dv(buf[1].x);
			glVertex3dv(buf[2].x);
			glVertex3dv(buf[3].x);
//			for (k=0; k<4; k++) {
//				for (l=0; l<3; l++) {
//					v[k][l] = basicVector[k].x[l];
//				}
//			}
//			glVertex3dv(v[0]);
//			glVertex3dv(v[1]);
//			glVertex3dv(v[2]);
//			glVertex3dv(v[3]);
		}
	}
	glEnd();
//	
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, colors[WHITE]);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, colors[BLACK]);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, colors[WHITE]);
//	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
//	glTranslatef(square->zeroNode.x[0], square->zeroNode.x[1], square->zeroNode.x[2]);
//	glBegin(GL_QUADS);
//	for (i = 1; i < square->paintSquare.numberOfElement[0]; i++){
//		for (j = 0; j < square->paintSquare.numberOfElement[1]; j++){
//			if (!((square->paintSquare.state[i][j]) & 2))continue;
//			copyVector(&basicVector[0], &square->basicVector[0]);
//			copyVector(&basicVector[1], &square->basicVector[0]);
//			copyVector(&basicVector[2], &square->basicVector[1]);
//			copyVector(&basicVector[3], &square->basicVector[1]);
//			changeLengthOfVector(&basicVector[0], i*PAINTCELL_SIZE);
//			changeLengthOfVector(&basicVector[1], (i+1)*PAINTCELL_SIZE);
//			changeLengthOfVector(&basicVector[2], j*PAINTCELL_SIZE);
//			changeLengthOfVector(&basicVector[3], (j+1)*PAINTCELL_SIZE);
//			copyVector(&buf[0], &basicVector[0]);
//			addVector(&buf[0], &basicVector[2]);
//			copyVector(&buf[1], &basicVector[1]);
//			addVector(&buf[1], &basicVector[2]);
//			copyVector(&buf[2], &basicVector[1]);
//			addVector(&buf[2], &basicVector[3]);
//			copyVector(&buf[3], &basicVector[0]);
//			addVector(&buf[3], &basicVector[3]);
//			glVertex3dv(buf[0].x);
//			glVertex3dv(buf[1].x);
//			glVertex3dv(buf[2].x);
//			glVertex3dv(buf[3].x);
//		}
//	}
//	glEnd();
	
	glPopMatrix();
	
//	free(buf);
//	free(basicVector);
	
}

void drawCuboid(Cuboid *cuboid){
	int i, j;
	
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colors[GRAY]);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, colors[BLACK]);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, colors[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
//	printf("1:\n");
	for (i = 0; i < 6; i++){
		glBegin(GL_QUADS);
		glNormal3dv(cuboid->normalvec[i]);
		for (j = 0; j < 4; j++){
			glVertex3dv(cuboid->node[cuboid->face[i][j]]);
		}
		glEnd();
	}
	glPopMatrix();
}

void drawCuboidPaintableFace(Cuboid *cuboid){
	int i;
	for (i=0; i<6; i++) {
		if (cuboid->paintableFaces[i].isPaintable) {
			drawPaintSquare(&cuboid->paintableFaces[i].squareFace);
		}
	}
}







