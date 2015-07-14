#ifdef __APPLE__
#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <math.h>
#include "game.h"


int main(int argc, char *argv[])
{
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Splateen");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}
