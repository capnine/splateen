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
	{ 1.0, 0.553, 0.156, 1.0   },
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
void copyNode(Node *settedNode,Node *sourceNode){
	copyVector(&settedNode->position, &sourceNode->position);
}
////////////////////////
/*        Squre       */
////////////////////////

//引数のnodesは面の表から見て反時計回りの順番でなくてはならない
void initSquareWith4Nodes(Square *square,Node nodes[]){
	int i;
	double length;
	Vector *buf;
	buf = (Vector *)malloc(sizeof(Vector));
	
	//squareのnodesをセット
	for (i=0; i<4; i++) {
		initNode(&square->nodes[i], nodes[i].position.x);
	}
	
	//squareのゼロ点(基準点)をセット
	copyVector(&square->zeroNode, &nodes[0].position);
	
	//basicVector[0]とsize[0]をセット
	copyVector(buf, &nodes[1].position);
	minusVector(buf, &nodes[0].position);
	length = getValueOfVector(buf);
	square->size[0] = length;
	changeLengthOfVector(buf, 1.0/length);
	copyVector(&square->basicVector[0], buf);
	
	//basicVector[1]とsize[1]をセット
	copyVector(buf, &nodes[3].position);
	minusVector(buf, &nodes[0].position);
	length = getValueOfVector(buf);
	square->size[1] = length;
	changeLengthOfVector(buf, 1.0/length);
	copyVector(&square->basicVector[1], buf);
	
	//法線ベクトルをセット
	setNormalVector(&square->normalVector, &square->basicVector[0], &square->basicVector[1]);
	
	initPaintSquare(&square->paintSquare, square->size);
	free(buf);
}

void printSquare(Square *squre){
//	printf("pr sq\n");
	printf("ZeroNode:\t");
	printVector(&squre->zeroNode);
	printf("basicVector[0]:\t");
	printVector(&squre->basicVector[0]);
	printf("basicVector[1]:\t");
	printVector(&squre->basicVector[1]);
	printf("size:%4f\t%4f\n",squre->size[0],squre->size[1]);
	printf("\n");
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

void initCuboidWithSize3dAndPosition3d(Cuboid *cuboid,double size[],double position[]){
	int i,j;
	for (i=0; i<3; i++)	cuboid->position.x[i]=position[i];
	for (i=0; i<3; i++)	cuboid->size3d[i]=size[i];
	cuboid->color=0;
	setNode(cuboid);
	
	for(i=0;i<6;i++){
		for(j=0;j<4;j++){
			cuboid->face[i][j]=normalFace[i][j];
		}
	}
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
	Node *buf;
	buf = (Node *)malloc(sizeof(Node)*4);
	for (i=0; i<6; i++) {
		for (j=0; j<4; j++) {
			copyNode(&buf[j], &cuboid->nodes[cuboid->face[i][j]]);
		}
		initCuboidFace(&cuboid->paintableFaces[i], buf);
	}
	free(buf);
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

void paintSquare(Square *square,double xy[]){
	int i,j;
	double p,q,r;
	for (i=0; i<square->paintSquare.numberOfElement[0]; i++) {
		for (j=0; j<square->paintSquare.numberOfElement[1]; j++) {
			p = (double)(i+1.0/2.0) * PAINTCELL_SIZE - xy[0];
			q = (double)(j+1.0/2.0) * PAINTCELL_SIZE - xy[1];
			r = sqrt(p*p+q*q);
			if (r<PAINT_SIZE) {
				square->paintSquare.state[i][j] = 1;
			}
		}
	}
}

void drawPaintSquare(Square *square){
	int i,j;
	Vector *offset;
	Vector *buf;
	Vector *basicVector;
	offset = (Vector *)malloc(sizeof(Vector));
	buf = (Vector *)malloc(sizeof(Vector)*4);
	basicVector = (Vector *)malloc(sizeof(Vector)*4);
	
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colors[ORANGE]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colors[ORANGE]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colors[ORANGE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glTranslatef(square->zeroNode.x[0], square->zeroNode.x[1], square->zeroNode.x[2]);
	copyVector(offset, &square->normalVector);
	changeLengthOfVector(offset, 0.001);
	glTranslatef(offset->x[0], offset->x[1], offset->x[2]);
	glBegin(GL_QUADS);
	for (i = 0; i < square->paintSquare.numberOfElement[0]; i++){
		for (j = 0; j < square->paintSquare.numberOfElement[1]; j++){
			if (!((square->paintSquare.state[i][j]) & 1))continue;
//			if ((i^j)&1) continue;
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
		}
	}
	glEnd();

//	glMaterialfv(GL_FRONT, GL_DIFFUSE, colors[WHITE]);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, colors[BLACK]);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, colors[WHITE]);
//	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
//	glTranslatef(square->zeroNode.x[0], square->zeroNode.x[1], square->zeroNode.x[2]);
//	glBegin(GL_QUADS);
//	for (i = 1; i < square->paintSquare.numberOfElement[0]; i++){
//		for (j = 0; j < square->paintSquare.numberOfElement[1]; j++){
//			if (!((square->paintSquare.state[i][j]) & 1))continue;
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
	
	free(offset);
	free(buf);
	free(basicVector);
	
}

void drawCuboid(Cuboid *cuboid){
	int i, j;
	
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colors[GRAY]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colors[BLACK]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colors[WHITE]);
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
void printCuboid(Cuboid *cuboid){
	int i;
	for (i=0; i<6; i++) {
		printf("face:%d\n",i);
		printSquare(&cuboid->paintableFaces[i].squareFace);
	}
}







