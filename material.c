#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
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
/*       Cuboid       */
////////////////////////


int normalFace[6][4]={
	{0,1,2,3},
	{4,5,6,7},
	{0,1,5,4},
	{1,2,6,5},
	{2,3,7,6},
	{3,0,4,7}
};

void setNode(Cuboid *cuboid){
	int i,j;
	//直方体の８頂点を設定する基本ベクトル
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
			cuboid->node[i][j]=(double)cuboid->size3d[j]*vector[i][j];
		}
	}
}

void initCuboid(Cuboid *cuboid){
	int i,j;
	double* posi = cuboid->position.x;
	posi[0]=0.0;
	posi[1]=0.0;
	posi[2]=0.0;
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
	setNode(cuboid);
}

void setCuboidPosition(Cuboid *cuboid,double x,double y,double z){
	double* posi = cuboid->position.x;
	posi[0]=x;
	posi[1]=y;
	posi[2]=z;
	setNode(cuboid);
}

//void setCuboidParameter(Cuboid *cuboid){
//	int i;
//	for (i=0; i<6; i++) {
//		setVector(cuboid->paintableFaces[i].face.zeroNode,
//	}
//}

void drawCuboid(Cuboid *cuboid){
	int i, j,k;
	double *posi;
	GLdouble v[4][3];
	GLdouble normal[3];

	glPushMatrix();
	posi = cuboid->position.x;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colors[GRAY]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colors[BLACK]);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, colors[WHITE]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glTranslated(posi[0], posi[1], posi[2]);
	for (i = 0; i < 6; i++){
		glBegin(GL_QUADS);
		for (j = 0; j < 4; j++){
			for(k=0;k<3;k++)	v[j][k] =cuboid->node[cuboid->face[i][j]][k];
			glVertex3dv(v[j]);
		}
		calcNormal(v[0],v[1],v[2],normal);
		glNormal3dv(normal);
		glEnd();
	}

	glPopMatrix();
}

void initNode(Node *node, double x[3]){
	setVector(&node->position, x);
}








