#ifndef PHYSICS_H
#define PHYSICS_H

#define PHYSICS_G 9.8
#define PHYSICS_MARGIN 0.0001
#define PI 3.141592

#include "material.h"

typedef struct {
	double x[3];
}Vector;

void calcNormal(GLdouble v0[3], GLdouble v1[3], GLdouble v2[3], GLdouble n[3]);
void setVector(Vector *vector,double x,double y,double z);
void rotateVector(double vector[3], double rotateVector[3]);
double innerVector(Vector *v1s,Vector *v2);//内積
void crossVector(Vector *v1,Vector *v2, Vector *ans);//外積
double distanceBetweenVectors(Vector *v1,Vector *v2);
//int collidionWithCuboid(Cuboid *cuboid,Player *player);

#endif