#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "physics.h"


void setVector(Vector *vector,double x[3]){
	int i;
	for(i=0;i<3;i++){
		vector->x[i] = x[i];
	}
	vector->value = getValueOfVector(vector);
}

void copyVector(Vector *settedVector,Vector *sourceVector){
	setVector(settedVector, sourceVector->x);
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

void setNormalVector(Vector *settedVecor,Vector *basicVector1,Vector *basicVector2){
	double length;
	crossProduct(settedVecor, basicVector1, basicVector2);
	length = getValueOfVector(settedVecor);
	changeLengthOfVector(settedVecor, 1.0/length);
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








