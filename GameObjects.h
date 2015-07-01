#ifndef __gltest__GameObjects__
#define __gltest__GameObjects__

#include "physics.h"
#include "material.h"

typedef struct {
	double distance;
	Vector position;
	Vector pointLookedAt;
	Vector head;
}Camera;

typedef struct{
	int state;
	Vector position;
	double height;
	double radius;
}Player;

typedef struct {
	Cuboid cuboids[10];
	int numberOfCuboid;//直方体の数
}Stage;

void initCamera(Camera *camera,Player *player);
void moveCamera(Camera *camera,Player *player,int command);
void lookByCamera(Camera *camera);

void initPlayer(Player *player);
void movePlayer(Player *player,Stage *stage,int command);
void drawPlayer(Player *player);
int collidionWithCuboid(Cuboid *cuboid,Player *player);


void initStage(Stage *stage);
void drawStage(Stage *stage);

#endif /* defined(__gltest__GameObjects__) */
