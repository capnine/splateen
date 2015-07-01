#ifndef MATERIAL_H
#define MATERIAL_H

#define AMOUNT_OF_CUBOIDS 10

#include "physics.h"

enum COLOR { WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, GRAY, BLACK };
extern GLfloat color[][4];

typedef struct{
	double position3d[3];
	double size3d[3];
	int color;
	double node[8][3];
	int face[6][4];
}Cuboid;

typedef struct{
	double position3d[3];
	double height;
	double radius;
	int color;
	double velocity[3];
}Player;

typedef struct {
	Cuboid cuboids[AMOUNT_OF_CUBOIDS];
}Stage;

void initCuboid(Cuboid *cuboid);
void setCuboidSize3d(Cuboid *cuboid,double x,double y,double z);
void setCuboidPosition3d(Cuboid *cuboid,double x,double y,double z);
void drawCuboid(Cuboid *cuboid);

void initStage(Stage *stage);
void drawStage(Stage *stage);

void initPlayer(Player *player);
void setPlayerPosition(Player *player,double x, double y, double z);
void drawPlayer(Player *player);

void drawCylinder(float radius,float height,int sides);

#endif