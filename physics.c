#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "physics.h"

void initMatrix3dWith3Row(Matrix *matrix,double rows1[],double rows2[],double rows3[]){
	int i;
	matrix->n = 3;
	for (i=0; i<3; i++) {
		matrix->a[0][i] = rows1[i];
		matrix->a[1][i] = rows2[i];
		matrix->a[2][i] = rows3[i];
	}
}

void copyMatrix(Matrix *settedMatrix,Matrix *sourceMatrix){
	int i,j,n;
	n = sourceMatrix->n;
	settedMatrix->n = n;
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			settedMatrix->a[i][j] = sourceMatrix->a[i][j];
		}
	}
}

void changeRowsOfMatrix(Matrix *A,int a,int b){
	int i,n;
	double buf[5];
	n = A->n;
	if (a>n || b>n) {
		printf("入力が正しくないです(changeRowsOfMatrix)\n");
		return;
	}
	for (i=0; i<n; i++) {
		buf[i] = A->a[a][i];
		A->a[a][i] = A->a[b][i];
		A->a[b][i] = buf[i];
	}
}

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
	Vector *buf;
	buf = (Vector *)malloc(sizeof(Vector));
	double theta = 2*PI*angle/360.0;
	setVector(buf, rotatedVector->x);
	rotatedVector->x[0]= cos(theta) * buf->x[0] - sin(theta) * buf->x[1];
	rotatedVector->x[1]= sin(theta) * buf->x[0] + cos(theta) * buf->x[1];
	free(buf);
}

void printVector(Vector *a){
	int i;
	for (i=0; i<3; i++) {
		printf("%4.2f\t",a->x[i]);
	}
	printf("\n");
}

void solveSimultaneousEquation(Matrix *A,double x[],double b[]){
	int i,j,k;
	int n;
	double p,q,c;
	Matrix *bufA;
	double bufb[5];
	
	bufA = (Matrix *)malloc(sizeof(Matrix));
	n=A->n;
	if (n > 3) {
		printf("行列の次元が３でないのでとけませ〜ん\n");
		free(bufA);
		return;
	}
	
	copyMatrix(bufA, A);
	for (i=0; i<n; i++) bufb[i] = b[i];
	
	for (i=0; i<n; i++) {
		if (bufA->a[i][i] == 0.0) {
			if (i==n-1) {
				printf("とけません(solveSimultaneousEquation)\n");
				free(bufA);
				return;
			}
			for (j=i+1; j<n; j++) {
				if (bufA->a[j][i] != 0.0) {
					changeRowsOfMatrix(bufA, i, j);
					c = bufb[i];
					bufb[i] = bufb[j];
					bufb[j] = c;
					break;
				}
			}
		}
		p = bufA->a[i][i];
		for (j=i; j<n; j++){
			bufA->a[i][j] /= p;
		}
		bufb[i] /= p;
		if (i==n-1) {
			break;
		}
		for (j=0; j<n; j++) {
			if (j==i) continue;
			q = bufA->a[j][i];
			for (k=i; k<n; k++) {
				bufA->a[j][k] -= q*bufA->a[i][k];
			}
			bufb[j] -= q*bufb[i];
		}
	}
	for (i=n-1; i>=1; i--) {
		for (j=0; j<i; j++) {
			q = bufA->a[j][i];
			bufA->a[j][i] -= q*bufA->a[i][i];
			bufb[j] -= q*bufb[i];
		}
	}
	
	for (i=0; i<n; i++) {
		x[i] = bufb[i];
	}
	free(bufA);
}
























