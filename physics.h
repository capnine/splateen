#ifndef PHYSICS_H
#define PHYSICS_H

#define PI 3.14159
#define MARGIN 0.001
#define PHYSICS_G (9.8*0.007)

typedef struct{
	double x[3];
	double value;//長さ、ベクトル値
}Vector;//ベクトル

typedef struct {
	int n;//n次正方行列
	double a[5][5];//要素(5次元まで)
	double value;//行列式
}Matrix;

void initMatrix3dWith3Row(Matrix *matrix,double rows1[],double rows2[],double rows3[]);
void copyMatrix(Matrix *settedMatrix,Matrix *sourceMatrix);
void changeRowsOfMatrix(Matrix *A,int a,int b);//行列のa列とb列を入れ替える
void setVector(Vector *vector,double x[3]);//x配列でベクトルをセットする
void copyVector(Vector *settedVector,Vector *sourceVector);//ベクトルをコピーする
double innerVector(Vector *a,Vector *b);//2本のベクトルの内積を返す
void crossProduct(Vector *settedVector,Vector *a,Vector *b);//第一引数のベクトルに外積をセットする
void addVector(Vector *addedVector,Vector *a);//第一引数に第二引数のベクトルを足す
void minusVector(Vector *minusedVector,Vector *a);//第一引数に第二引数のベクトルを引く
double getValueOfVector(Vector *a);//ベクトルの長さを返す
double distanceBetweenVectors(Vector *a,Vector *b);//ベクトルの距離を返す
void changeLengthOfVector(Vector *a, double rate);//ベクトルの長さをrate倍にする
void setNormalVector(Vector *settedVecor,Vector *basicVector1,Vector *basicVector2);//2つのベクトルからもとめた法線ベクトルをセットする
void rotateVectorInXY(Vector *rotatedVector,double angle);//ベクトルをz軸を軸に(xy平面上で)angle度回転させる。
void printVector(Vector *a);//ベクトルをコンソールに表示

double getMatrixValue3d(Matrix *A);
void setReverseMatrix3d(Matrix *settedMatrix,Matrix *sourceMatrix);

void solveSimultaneousEquation(Matrix *A,double x[],double b[]);//A*x=bの行列を解く、解はxに代入される。
#endif