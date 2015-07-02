#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include "physics.h"


void setVector(Vector *vector,double x[3]){
	int i;
	for(i=0;i<3;i++){
		vector->x[i] = x[i];
	}
}

void setVectorWithXYZ(Vector *vector,double x,double y,double z){
	vector->x[0]=x;
	vector->x[1]=y;
	vector->x[2]=z;
}

double innerVector(Vector *a,Vector *b){
	int i;
	double sum=0;
	for (i=0; i<3; i++) {
		sum += a->x[i] * b->x[i];
	}
	return sum;
}

void crossProduct(Vector *settedVector,Vector *a,Vector *b){
	int i;
	double ans[3];
	for (i=0; i<3; i++) {
		ans[i] = a->x[(i+1)%3] * b->x[(i+2)%3] -  a->x[(i+2)%3] * b->x[(i+1)%3];
	}
	setVector(settedVector, ans);
}

void addVector(Vector *addedVector,Vector *a){
	int i;
	for (i=0; i<3; i++) {
		addedVector->x[i] += a->x[i];
	}
}
void minusVector(Vector *minusedVector,Vector *a){
	int i;
	for (i=0; i<3; i++) {
		minusedVector->x[i] -= a->x[i];
	}
}

double getValueOfVector(Vector* a){
	int i,r;
	double sum=0;
	for (i=0; i<3; i++) {
		r = a->x[i];
		sum += r * r;
	}
	return sqrt(sum);
}

double distanceBetweenVectors(Vector *a,Vector *b){
	int i;
	double sum = 0;
	double d;
	for (i=0; i<3; i++) {
		d = a->x[i] - b->x[i];
		sum += d * d;
	}
	return sqrt(sum);
}

void changeLengthOfVector(Vector *a, double rate){
	int i;
	for (i=0; i<3; i++) {
		a->x[i] *= rate;
	}
}

void getNormalVector(Vector *settedVecor,Vector *basicVector1,Vector *basicVector2){
	double length;
	crossProduct(settedVecor, basicVector1, basicVector2);
	length = getValueOfVector(settedVecor);
	changeLengthOfVector(settedVecor, 1.0/length);
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

void rotateVectorInXY(Vector *rotatedVector,double angle){
	Vector buf;
	double theta = 2*PI*angle/360.0;
	setVector(&buf, rotatedVector->x);
	rotatedVector->x[0]= cos(theta) * buf.x[0] - sin(theta) * buf.x[1];
	rotatedVector->x[1]= sin(theta) * buf.x[0] + cos(theta) * buf.x[1];
}

void printVector(Vector *a){
	int i;
	for (i=0; i<3; i++) {
		printf("%4.2f\t",a->x[i]);
	}
	printf("\n");
}








