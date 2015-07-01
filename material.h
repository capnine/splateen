#ifndef MATERIAL_H
#define MATERIAL_H

typedef struct{
	double position3d[3];
	double size3d[3];
	int color;
	char isVisible;
	double node[8][3];//’¸“_
	int face[6][4];//–Ê
}Cuboid;
void initCuboid(Cuboid *cuboid);
void setSize3d(Cuboid *cuboid,double x,double y,double z);
void setPosition3d(Cuboid *cuboid,double x,double y,double z);
void drawCuboid(Cuboid *cuboid);

#endif