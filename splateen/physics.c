#include <math.h>
#include <GLUT/glut.h>
#include "physics.h"

void setVector(Vector *vector,double x,double y,double z)
{
	vector->x[0]=x;
	vector->x[1]=y;
	vector->x[2]=z;
}


void calcNormal(GLdouble v0[3], GLdouble v1[3], GLdouble v2[3], GLdouble n[3])
{
	GLdouble v2v[2][3];
	GLdouble vt[3];
	double abs;
	int i;
	for (i = 0; i < 3; i++)
	{
		v2v[0][i] = v1[i] - v0[i];
		v2v[1][i] = v2[i] - v0[i];
	}
	vt[0] = v2v[0][1] * v2v[1][2] - v2v[0][2] * v2v[1][1];
	vt[1] = v2v[0][2] * v2v[1][0] - v2v[0][0] * v2v[1][2];
	vt[2] = v2v[0][0] * v2v[1][1] - v2v[0][1] * v2v[1][0];
	abs = sqrt(vt[0] * vt[0] + vt[1] * vt[1] + vt[2] * vt[2]);
	for (i = 0; i < 3; i++)
		n[i] = vt[i] / abs;
}

double innerVector(Vector *v1, Vector *v2){
	double scalar = 0;
	int i;
	for (i=0; i<3; i++) {
		scalar += v1->x[i] * v2->x[i];
	}
	return scalar;
}

void crossVector(Vector *v1,Vector *v2,Vector *ans){
	int i;
	for (i=0; i<3; i++)	ans->x[i] = v1->x[(i+1)%3] * v2->x[(i+2)%3] - v1->x[(i+2)%3] * v2->x[(i+1)%3];
}

double distanceBetweenVectors(Vector *v1,Vector *v2){
	double r2=0;
	double dx;
	int i;
	for (i=0; i<3; i++) {
		dx = v1->x[i] - v2->x[i];
		r2 += dx * dx;
	}
	return sqrt(r2);
}


int collidionWithCuboid(Cuboid *cuboid,Player *player){
	int i;
	int flag1=0;
	int flag2=0;
	int flag3=0;
	int flag4=0;
	double player_x = player->position3d[0];
	double player_y = player->position3d[1];
	double player_r = player->radius;
	double cube_maxX = cuboid->node[6][0];
	double cube_minX = cuboid->node[0][0];
	double cube_maxY = cuboid->node[6][1];
	double cube_minY = cuboid->node[0][1];
	double cube_maxZ = cuboid->node[6][2];
	double cube_minZ = cuboid->node[0][2];
	
	if (((cube_maxZ > player->position3d[2]) && (cube_minZ < player->position3d[2]))||
		((cube_maxZ > (player->position3d[2] + player->height)) && (cube_minZ < (player->position3d[2] + player->height)))) {
		flag1 = 1;
	}
	Vector vector1;
	Vector vector2;
	setVector(&vector1, player_x, player_y, 0.0);
	for (i=0; i<4; i++) {
		setVector(&vector2, cuboid->node[i][0], cuboid->node[i][1], 0.0);
		if (distanceBetweenVectors(&vector1, &vector2) < (player_r + PHYSICS_MARGIN)) {
			flag2 ++;
		}
	}
	
	if (((cube_maxX + player_r + PHYSICS_MARGIN) >  player_x )&&
		((cube_minX - player_r - PHYSICS_MARGIN) < player_x )&&
		((cube_maxY + PHYSICS_MARGIN) >  player_y )&&
		((cube_minY - PHYSICS_MARGIN) < player_y )){
			flag3 ++;
		}
	if (((cube_maxX + PHYSICS_MARGIN) >  player_x )&&
		((cube_minX - PHYSICS_MARGIN) < player_x )&&
		((cube_maxY + player_r + PHYSICS_MARGIN) >  player_y)&&
		((cube_minY - player_r - PHYSICS_MARGIN) < player_y)){
			flag4 ++;
		}
	
		printf("1:%d 2:%d 3:%d 4:%d\n",flag1,flag2,flag3,flag4);
	return flag1 && (flag2 || flag3 || flag4);
}








