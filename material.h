#ifndef MATERIAL_H
#define MATERIAL_H

#include "physics.h"

#define SIZE_OF_PAINT 0.1

enum COLOR { WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, GRAY, BLACK };
extern GLfloat colors[][4];

typedef struct {
	Vector position;
}Node;//点

typedef struct{
	Vector normalVector;//法線
	Vector zeroNode;//基準点
	Vector basicVector[2];//基準点から隣の頂点へのベクトル
	Node nodes[4];
}Square;//面

typedef struct{
	int numberOfElement;
}PaintSquare;//塗れる面

typedef struct{
	Square squareFace;
	PaintSquare paintFace;
}CuboidFace;//直方体の面(色がぬれる)

typedef struct{
	Vector position;//位置は一番座標が小さな値が基準
	Vector maxPosition;
	double size3d[3];
	int color;
	char isVisible;
	double node[8][3];
	int face[6][4];
	CuboidFace paintableFaces[6];
}Cuboid;

void initNode(Node *node, double x[3]);
void initSquareWith4Nodes(Square *square,Node nodes[]);
void initCuboidFace(CuboidFace *cuboidFace,Node nodes[]);

void initCuboid(Cuboid *cuboid);
void setCuboidSize(Cuboid *cuboid,double x,double y,double z);
void setCuboidPosition(Cuboid *cuboid,double x,double y,double z);
void setCuboidMaxPosition(Cuboid *cuboid);
void setCuboidAllParameter(Cuboid *cuboid);
void drawCuboid(Cuboid *cuboid);


#endif