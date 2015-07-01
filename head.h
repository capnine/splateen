#ifndef PART_H
#define PART_H

enum COLOR { WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, GRAY, BLACK };

extern GLfloat pos0[];
extern GLfloat pos1[];
extern GLfloat color[][4];
extern int X;
extern int Y;
extern double L;
extern double x;
extern double y;
extern double z;
extern int mySpecialValue;
extern double tekiList[][3];
extern int tekiIndex;
extern double v;
extern double theta;


void calcNormal(GLdouble v0[3], GLdouble v1[3], GLdouble v2[3], GLdouble n[3]);
void drawJiki();
void drawTeki();
void drawGround();

int collision();
void display(void);
void myTimerFunc(int value);
void myKeyboardFunc(unsigned char key, int x, int y);
void mySpcialFunc(int key, int x, int y);
void mySpcialUpFunc(int key, int x, int y);
void init(void);
#endif
