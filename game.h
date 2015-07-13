#ifndef PART_H
#define PART_H

#include "GameObjects.h"

#define PLAYER_COLOR 3
#define COMP_COLOR 4
#define MAX_GAME_TIME 2000

extern BulletList *bulletList;
extern Vector *firstPlayerPosition;
extern Vector *firstCompPosition;

void display(void);
void myTimerFunc(int value);void init(void);

#endif
