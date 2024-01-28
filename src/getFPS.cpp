#define _CRT_SECURE_NO_WARNINGS
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

void renderCenterPoint(int Width,int Height,bool ifCenterPoint){
    if (!ifCenterPoint) return;
    glPushMatrix();
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, Width, 0, Height);
    glMatrixMode(GL_MODELVIEW);
    Material::SetColor(1, 0, 0);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex3f((Width - 80) / 2 + 80, Height / 2, 0.0f);
    glEnd();
    glPopMatrix();
 }

void renderAxis(int Width, int Height,float pitch,float yaw, bool ifAxiz) {
    if (!ifAxiz) return;
    glPushMatrix();
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, Width, 0, Height, -200, 200);
    glMatrixMode(GL_MODELVIEW);

    /*glTranslatef(-30,0,-30);*/
    glTranslatef(-Width / 4, 0, 0);
    glTranslatef((Width - 80) / 2 + 80, Height / 2, 1.0f);    // 调整绘图坐标
    glRotatef(pitch, 1, 0, 0);
    glRotatef(yaw + 90, 0, 1, 0);
    // 坐标轴
    float axis_length = 100;
    glLineWidth(5);
    glBegin(GL_LINE_STRIP);
    Material::SetColor(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(axis_length, 0, 0);
    glEnd();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0); // 设置颜色为红色
    glTranslatef(axis_length, 0, 0);
    glRotatef(90, 0, 1, 0);
    glutSolidCone(5.0, 10.0, 50, 50);
    glPopMatrix();

    glBegin(GL_LINE_STRIP);
    Material::SetColor(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, axis_length, 0);
    glEnd();
    glPushMatrix();
    glTranslatef(0, axis_length, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(5.0, 10.0, 50, 50); // 绘制圆锥
    glPopMatrix();

    glBegin(GL_LINE_STRIP);
    Material::SetColor(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, axis_length);
    glEnd();
    glPushMatrix();
    glTranslatef(0, 0, axis_length);
    glRotatef(90, 0, 0, 0);
    glutSolidCone(5.0, 10.0, 50, 50); // 绘制圆锥
    glPopMatrix();

    glPushMatrix();
    Material::SetColor(1, 1, 1);
    glutSolidSphere(5.0f, 100,100);
    glPopMatrix();
    glPopMatrix();
}