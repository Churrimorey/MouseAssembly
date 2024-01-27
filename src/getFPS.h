#pragma once
#include <string.h>
#include <stdio.h>
#include <gl/glut.h>
#include "material.h"
void renderBitmapString(float x, float y, void* font, char* string);
void getFPS();
void renderCenterPoint(int Width, int Height,bool ifCenterPoint);
void renderAxis(int Width, int Height, float pitch, float yaw, bool ifAxiz);