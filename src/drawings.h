#ifndef __DRAWINGS_H_
#define __DRAWINGS_H_

#include <gl/glut.h>
#include <cmath>

#define PI 3.14159

extern unsigned int texture[2];
extern GLint holeList[25];
extern GLint batterList[25];

void Texture_desk();
void DrawTable();
void DrawCircle(float cx, float cy, float cz, float r, int num_segments);
void DrawCuboid(double cx, double cy, double cz, double halfX, double frontY, double backY, double halfZ);
void DrawDesk();
void DrawHole();
void DrawHoles();
void DrawBattery();
void DrawBatterys();
void DrawBox();
void GenHoleList();
void DrawHoleList();

#endif
