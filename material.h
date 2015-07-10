#ifndef MATERIAL_H
#define MATERIAL_H

#include "physics.h"

//Paintsquareの塗るセルの細かさ
#define PAINTCELL_SIZE 0.1

enum COLOR { WHITE, RED, GREEN, BLUE, ORANGE, MAGENTA, CYAN, GRAY, BLACK };
extern GLfloat colors[][4];

typedef struct {
	Vector position;
}Node;//頂点

typedef struct{
	int numberOfElement[2];//SquareのbasicVectorを基準とした塗るセルの要素数
	char state[128][128];//濡れる面は最大128*128セルよって大きさは12.8*12.8が限界(0:塗られていない、n:グループnによって塗られている)
}PaintSquare;//塗れる面

typedef struct{
	Vector normalVector;//法線
	Vector zeroNode;//基準点
	Vector basicVector[2];//基準点から隣の頂点への基底ベクトル
	double size[2];//基底ベクトルを基準とした四角形の大きさ
	Node nodes[4];//頂点
	PaintSquare paintSquare;
}Square;//面

typedef struct{
	Square squareFace;
	char isPaintable;
}CuboidFace;//直方体の面(色がぬれる)

typedef struct{
	Vector position;//位置は一番座標が小さな値が基準
	Vector maxPosition;
	CuboidFace paintableFaces[6];
	Node nodes[8];//頂点
	double size3d[3];//大きさ[x,y,z]
	
	//draw高速化用
	int color;
	char isVisible;
	GLdouble node[8][3];
	int face[6][4];
	GLdouble normalvec[6][3];
}Cuboid;

void initNode(Node *node, double x[3]);
void initSquareWith4Nodes(Square *square,Node nodes[]);
void initCuboidFace(CuboidFace *cuboidFace,Node nodes[]);
void initPaintSquare(PaintSquare *paintSquare,double size[]);

void initCuboid(Cuboid *cuboid);
void setCuboidSize(Cuboid *cuboid,double x,double y,double z);
void setCuboidPosition(Cuboid *cuboid,double x,double y,double z);
void setCuboidMaxPosition(Cuboid *cuboid);
void setCuboidCuboidFace(Cuboid *cuboid);
void setCuboidNormalvec(Cuboid *cuboid);
void setCuboidAllParameter(Cuboid *cuboid);
void drawPaintSquare(Square *square);
void drawCuboid(Cuboid *cuboid);
void drawCuboidPaintableFace(Cuboid *cuboid);


#endif