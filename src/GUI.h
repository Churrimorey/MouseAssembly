#pragma once
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <gl/glut.h>
#include "material.h"
#include <gl/glui.h>
#include "ScreenCapture.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "ScreenCapture.h"
void getFPS();
void renderCenterPoint(int Width, int Height, bool ifCenterPoint);
void renderAxis(int Width, int Height, float pitch, float yaw, int ifAxis);
void GUIinit();