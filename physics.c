#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "head.h"

void setVector(double vector[],double sourceVector[],int dimension){
	int i;
	for(i=0;i<dimension;i++){
		vector[i]=sourceVector[i];
	}
}