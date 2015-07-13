#ifndef MATERIAL_H
#define MATERIAL_H

#include "physics.h"

//Paintsquareの塗るセルの細かさ
#define PAINTCELL_SIZE 0.2
//着弾した時の塗る大きさ
#define PAINT_SIZE 4
#define PAINT_MAX_CELLS 516

enum COLOR { WHITE, RED, GREEN, BLUE, ORANGE, MAGENTA, CYAN, GRAY, BLACK };
extern GLfloat colors[][4];

typedef struct {
	Vector position;
}Node;//頂点

typedef struct{
	int numberOfElement[2];//SquareのbasicVectorを基準とした塗るセルの要素数
	char state[PAINT_MAX_CELLS][PAINT_MAX_CELLS];//濡れる面は最大516*516セルよって大きさは51.6*51.6が限界(0:塗られていない、n:グループnによって塗られている)
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
void copyNode(Node *settedNode,Node *sourceNode);
void initSquareWith4Nodes(Square *square,Node nodes[]);
void printSquare(Square *squre);
void paintSquare(Square *square,double xy[],double paintSize);
void drawPaintSquare(Square *square);

void initCuboidFace(CuboidFace *cuboidFace,Node nodes[]);
void initPaintSquare(PaintSquare *paintSquare,double size[]);

void initCuboidWithSize3dAndPosition3d(Cuboid *cuboid,double size[],double position[]);
void setCuboidMaxPosition(Cuboid *cuboid);
void setCuboidIsVisible(Cuboid *cuboid,char isVisible);
void setCuboidCuboidFace(Cuboid *cuboid);
void setCuboidNormalvec(Cuboid *cuboid);
void setCuboidAllParameter(Cuboid *cuboid);
void drawCuboid(Cuboid *cuboid);
void drawCuboidPaintableFace(Cuboid *cuboid);
void printCuboid(Cuboid *cuboid);


#endif