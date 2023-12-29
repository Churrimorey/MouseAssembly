#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdio.h>
#include <gl/glut.h>
#include "getFPS.h"

void renderBitmapString(float x, float y, void* font, char* string) {
    char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void getFPS() {
    static int frame = 0, time, timebase = 0;
    static char buffer[256];
    char mode[64];
    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        sprintf(buffer, "FPS:%4.2f ",
            frame * 1000.0 / (time - timebase), mode);
        timebase = time;
        frame = 0;
    }
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(700, 450, GLUT_BITMAP_HELVETICA_12, buffer);
}