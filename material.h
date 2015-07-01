#ifndef MATERIAL_H
#define MATERIAL_H

#include "physics.h"

#define SIZE_OF_PAINT 0.1

enum COLOR { WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, GRAY, BLACK };
extern GLfloat colors[][4];

typedef struct {
	Vector position;
}Node;//点
//
//typedef struct{
//	int numberOfNodes;//頂点の個数:四角形ならこれは４
//	Vector normalVector;//法線
//	Vector zeroNode;//基準点
//	Vector basicVector[2];//基準点から隣の頂点へのベクトル
//	Node nodes[4];
//}Face;//面
//
//typedef struct{
//	int numberOfElement;
//}PaintFace;//塗れる面
//
//typedef struct{
//	Face face;
//	PaintFace paintFace;
//}CuboidFace;//直方体の面(色がぬれる)

typedef struct{
	Vector position;//位置は一番座標が小さな値が基準
	double size3d[3];
	int color;
	char isVisible;
	double node[8][3];
	int face[6][4];
}Cuboid;


void initCuboid(Cuboid *cuboid);
void setSize3d(Cuboid *cuboid,double x,double y,double z);
void setPosition3d(Cuboid *cuboid,double x,double y,double z);
void drawCuboid(Cuboid *cuboid);

void initNode(Node *node, double x[3]);

#endif