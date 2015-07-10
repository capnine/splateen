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

void initMatrix3dWith3Column(Matrix *matrix,double column1[],double column2[],double column3[]){
	int i;
	matrix->n = 3;
	for (i=0; i<3; i++) {
		matrix->a[i][0] = column1[i];
		matrix->a[i][1] = column2[i];
		matrix->a[i][2] = column3[i];
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
	double sum=0.0;
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
	int i;
	double r,sum=0;
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
	printf("長さ:%4.2f\t",getValueOfVector(a));
	printf("\n");
}

double getMatrixValue3d(Matrix *A){
	int i;
	double sum=0.0;
	if (A->n != 3) {
		printf("行列の次元を確認してねin getMatrixValue3d\n");
		return -1;
	}
	for (i=0; i<3; i++) {
		sum += A->a[(i+0)%3][0] * A->a[(i+1)%3][1] * A->a[(i+2)%3][2];
		sum -= A->a[(i+0)%3][0] * A->a[(i+2)%3][1] * A->a[(i+1)%3][2];
	}
	return sum;
}

void setReverseMatrix3d(Matrix *settedMatrix,Matrix *sourceMatrix){
	int i;
	double delta;
	Matrix *A;
	A = sourceMatrix;
	delta = getMatrixValue3d(sourceMatrix);
	for (i=0; i<3; i++) {
		settedMatrix->a[i][0] =(A->a[1][(i+1)%3] * A->a[2][(i+2)%3] - A->a[1][(i+2)%3] * A->a[2][(i+1)%3])/delta;
		settedMatrix->a[i][1] =(A->a[0][(i+2)%3] * A->a[2][(i+1)%3] - A->a[0][(i+1)%3] * A->a[2][(i+2)%3])/delta;
		settedMatrix->a[i][2] =(A->a[0][(i+1)%3] * A->a[1][(i+2)%3] - A->a[0][(i+2)%3] * A->a[1][(i+1)%3])/delta;
	}
	settedMatrix->value = delta;
}

void solveSimultaneousEquation(Matrix *A,double x[],double b[]){
	int i,j;
	int n;
	Matrix *invA;
	
	invA = (Matrix *)malloc(sizeof(Matrix));
	n=A->n;
	if (n != 3) {
		printf("行列の次元が３でないのでとけませ〜ん\n");
		free(invA);
		return;
	}
	
	setReverseMatrix3d(invA, A);
	for (i=0; i<3; i++){
		x[i] = 0.0;
		for (j=0; j<3; j++) {
			x[i] += invA->a[i][j] * b[j];
		}
	}
	
	free(invA);
}
























