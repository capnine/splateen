#ifndef __gltest__GameObjects__
#define __gltest__GameObjects__

#include "physics.h"
#include "material.h"

#define PLAYER_G (9.8*0.007)
#define BULLET_G (0.01 )
#define BULLET_V 2
#define BULLET_RADIUS 0.2
#define MAX_BULLET 256

typedef struct {
	double distance;
	Vector position;
	Vector pointLookedAt;
	Vector head;
}Camera;

typedef struct{
	int state;//0:地面、1:空中
	int pauseCount;//この値が0以外の時はジャンプなどの入力を受け付けない
	Vector position;
	double height;
	double radius;
	Vector velocity;
	Vector acceleration;
	double lookAngleXY;
	double lookAngleZ;
}Player;

typedef struct{
	Vector position;
	Vector nextPosition;
	Vector velocity;
	double radius;
}Bullet;

typedef struct tagElement{
	struct tagElement *prev;
	Bullet *bullet;
	struct tagElement *next;
}BulletListElement;

typedef struct{
	BulletListElement *firstBulletElement;
	BulletListElement *lastBulletElement;
	BulletListElement bulletListElements[MAX_BULLET];
	int counter;
}BulletList;

typedef struct {
	Cuboid cuboids[10];
	int numberOfCuboid;//直方体の数
}Stage;

typedef struct {
	char move_up;
	char move_down;
	char move_right;
	char move_left;
	char look_right;
	char look_left;
	char look_up;
	char look_down;
	char jump;
}ActionFlag;

void initCamera(Camera *camera,Player *player);
void moveCamera(Camera *camera,Player *player);
void lookByCamera(Camera *camera);

void initPlayer(Player *player);
void setPlayerVelocity(Player *player);
void movePlayer(Player *player,Stage *stage,ActionFlag *af);
void drawPlayer(Player *player);
void movePlayerLookAngle(Player *player,ActionFlag *af);
void shotBullet(Player *player,BulletList *bulletList);
void printPlayer(Player *player);
int collidionWithCuboid(Cuboid *cuboid,Player *player);

void initBulletWithPlayer(Bullet *bullet,Player *player);
void moveBullet(Bullet *bullet);
void moveBullets(BulletList *bulletList);
void drawBullet(Bullet *bullet);
void drawBullets(BulletList *bulletList);

void initBulletList(BulletList *bulletList);
void addBullet(BulletList *bulletList,Bullet *bullet);

void initStage(Stage *stage);
void drawStage(Stage *stage);

void initActionFlag(ActionFlag *af);
void getActionFlag(ActionFlag *af,int mySpecialValue, int myKeyboardValue);

#endif /* defined(__gltest__GameObjects__) */
