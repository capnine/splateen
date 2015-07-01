#ifndef PHYSICS_H
#define PHYSICS_H

#define PI 3.14159
#define MARGIN 0.001

typedef struct{
	double x[3];
}Vector;//ベクトル

void setVector(Vector *vector,double x[3]);
double innerVector(Vector *a,Vector *b);//内積
void crossProduct(Vector *settedVector,Vector *a,Vector *b);//外積
void addVector(Vector *addedVector,Vector *a);//ベクトル和
double getValueOfVector(Vector *a);//ベクトルの長さ
void changeLengthOfVector(Vector *a, double rate);//rateは何倍にするか
void getNormalVector(Vector *settedVecor,Vector *basicVector1,Vector *basicVector2);
void calcNormal(GLdouble v0[3], GLdouble v1[3], GLdouble v2[3], GLdouble n[3]);
#endif