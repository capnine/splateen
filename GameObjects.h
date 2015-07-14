#ifndef __gltest__GameObjects__
#define __gltest__GameObjects__

#include "physics.h"
#include "material.h"

#define PLAYER_G (9.8*0.007)
#define PLAYER_SHOT_INTERVAL 10
#define PLAYER_V 0.15
#define BULLET_G (0.01)
#define BULLET_V 0.7
#define BULLET_RADIUS 0.5
#define MAX_BULLET 256
#define STAGE_MAX_X 128
#define STAGE_MAX_Y 128
#define STAGE_MAX_Z 10
#define MAX_CUBOIDS 127
#define DYING_TIME 200

enum PLAYER_STATE {ON_GRAND,IN_AIR,SWIMMING};

typedef struct {
	double distance;
	Vector position;
	Vector pointLookedAt;
	Vector head;
}Camera;

typedef struct{
	int color;
	int state;//0:地面、1:空中
	int pauseCount;//この値が0以外の時はジャンプなどの入力を受け付けない
	int shotPauseCount;//この値が0以外の時はshotできない
	int isComp;
	Vector position;
	Vector lowSpherePosition;
	Vector highSpherePosition;
	double height;
	double radius;
	Vector velocity;
	Vector acceleration;
	Vector lookVector;
	double lookAngleXY;
	double lookAngleZ;
	int dyingTime;
	int swimmingCuboid;
	int swimmingSquare;
	double swimmingPosition2d[2];
	double swimDistanceToSea;
}Player;

typedef struct{
	Vector position;
	Vector nextPosition;
	Vector velocity;
	double radius;
	int color;
	int index;
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
	Cuboid cuboids[MAX_CUBOIDS];
	int numberOfCuboid;//直方体の数
	double size[3];//ステージの大きさ、原点0を基準にこの座標より大きいオブジェクトは削除
}Stage;

typedef struct {
	int move_up;
	int move_down;
	int move_right;
	int move_left;
	int look_right;
	int look_left;
	int look_up;
	int look_down;
	int jump;
	int shot;
	int swim;
}ActionFlag;

void initCamera(Camera *camera,Player *player);
void moveCamera(Camera *camera,Player *player);
void lookByCamera(Camera *camera);

void initPlayer(Player *player);
void setPlayerVelocity(Player *player);
void setPlayerPosition(Player *player,Vector *position);
void setPlayerSpherePosition(Player *player);
void setPlayerLookVector(Player *player);
void actionPlayer(Player *player,Stage *stage,ActionFlag *af);
void movePlayer(Player *player,Stage *stage,ActionFlag *af);
void swimPlayer(Player *player,Stage *stage,ActionFlag *af);
void drawPlayer(Player *player);
void movePlayerLookAngle(Player *player,ActionFlag *af);
void shotBullet(Player *player,BulletList *bulletList);
void killPlayer(Player *player,Vector *initPosition);
void printPlayer(Player *player);
int collisionPlayerWithSquare(Player *player,Square *square);
int canSwimWithSquare(Player *player,Square *square);
int collisionPlayerWithCuboid(Player *player,Cuboid *cuboid);
int collisionPlayerWithBullet(Player *player,Bullet *bullet);
int collisionPlayerWithBullets(Player *player,BulletList *bulletList);

void initBulletWithPlayer(Bullet bullet[],Player *player);
void moveBullet(Bullet *bullet,BulletList *bulletList,Stage *stage);
void moveBullets(BulletList *bulletList,Stage *stage);
void drawBullet(Bullet *bullet);
void drawBullets(BulletList *bulletList);
int collisionBulletWithSquare(Bullet *bullet,Square *square);
int collisionBulletWithCuboid(Bullet *bullet,Cuboid *cuboid);

void initBulletList(BulletList *bulletList);
void addBullet(BulletList *bulletList,Bullet *bullet);
void removeBullet(BulletList *bulletList,int index);

void initStage(Stage *stage);
void drawStage(Stage *stage);
int getScore(Stage *stage,int color);

void initActionFlag(ActionFlag *af);
void getActionFlag(ActionFlag *af,int mySpecialValue, int myKeyboardValue);
void getCompAciton(ActionFlag *af);

#endif /* defined(__gltest__GameObjects__) */
