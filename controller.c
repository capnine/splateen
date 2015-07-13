#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "controller.h"

int mySpecialValue;
int myKeyboardValue;

void initController(void){
	myKeyboardValue = 0;
	mySpecialValue = 0;
}

void myKeyboardFunc(unsigned char key, int x, int y){
	switch (key){
		case ' ':
			myKeyboardValue |= 1 << 0;
			break;
		case 'w':
			myKeyboardValue |= 1 << 1;
			break;
		case 'a':
			myKeyboardValue |= 1 << 2;
			break;
		case 'd':
			myKeyboardValue |= 1 << 3;
			break;
		case 's':
			myKeyboardValue |= 1 << 4;
			break;
		case 'e':
			myKeyboardValue |= 1 << 5;
			break;
		case 'c':
			myKeyboardValue |= 1 << 6;
			break;
	}
}

void myKeyboardUpFunc(unsigned char key, int x, int y){
	switch (key){
		case ' ':
			myKeyboardValue &= ~(1 << 0);
			break;
		case 'w':
			myKeyboardValue &= ~(1 << 1);
			break;
		case 'a':
			myKeyboardValue &= ~(1 << 2);
			break;
		case 'd':
			myKeyboardValue &= ~(1 << 3);
			break;
		case 's':
			myKeyboardValue &= ~(1 << 4);
			break;
		case 'e':
			myKeyboardValue &= ~(1 << 5);
			break;
		case 'c':
			myKeyboardValue &= ~(1 << 6);
			break;
	}
}

void mySpcialFunc(int key, int x, int y){
//	printf("%d\n",key);
	switch (key){
		case GLUT_KEY_UP:
			mySpecialValue |= 1 << 0;
			break;
		case GLUT_KEY_LEFT:
			mySpecialValue |= 1 << 1;
			break;
		case GLUT_KEY_RIGHT:
			mySpecialValue |= 1 << 2;
			break;
		case GLUT_KEY_DOWN:
			mySpecialValue |= 1 << 3;
			break;
		default:
			break;
	}
}

void mySpcialUpFunc(int key, int x, int y){
	switch (key){
		case GLUT_KEY_UP:
			mySpecialValue &= ~(1 << 0);
			break;
		case GLUT_KEY_LEFT:
			mySpecialValue &= ~(1 << 1);
			break;
		case GLUT_KEY_RIGHT:
			mySpecialValue &= ~(1 << 2);
			break;
		case GLUT_KEY_DOWN:
			mySpecialValue &= ~(1 << 3);
			break;
		default:
			break;
	}
	
}

