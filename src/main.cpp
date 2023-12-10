#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gl/glut.h>
#include "mouse.h"
#include "utils.h"
#include "drawings.h"
#include "robot.h"

int gHeight;
int gWidth;
float eye[] = { 0, 4, 8 };
float center[] = { 0, 0, 0 };
unsigned int texture[2];
bool bMouseBase = false;
bool bMouseHead = false;
bool bBattery = false;
GLint holeList[25];
GLint batterList[25];

void init()
{
	glGenTextures(2, texture);
	texload(0, (char*)"table.bmp");
	texload(1, (char*)"battery.bmp");
}

void DrawScene()
{
	glColor3f(1.0 * 220 / 255, 1.0 * 220 / 255, 1.0 * 220 / 255);
	DrawTable();

	glColor3f(1.0 * 238 / 255, 1.0 * 121 / 255, 1.0 * 66 / 255);
	DrawLeftRobot();

	glColor3f(1.0 * 238 / 255, 1.0 * 121 / 255, 1.0 * 66 / 255);
	DrawRightRobot();

	glColor3f(1.0 * 156 / 255, 1.0 * 156 / 255, 1.0 * 156 / 255);
	DrawDesk();

	DrawBox();

	if (bBattery)
	{
		DrawBatterys();
	}

	DrawMousePlates();

	if (bMouseHead)
	{
		glColor3f(1.0 * 79 / 255, 1.0 * 79 / 255, 1.0 * 79 / 255);
		DrawMouseHeads();
	}

	if (bMouseBase)
	{
		glColor3f(1.0 * 79 / 255, 1.0 * 79 / 255, 1.0 * 79 / 255);
		DrawMouseBases();
	}

	glPushMatrix();
	glTranslatef(3.0f, -2.5f, 4.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.2f, 0.2f, 0.2f);
	glColor3f(1.0 * 79 / 255, 1.0 * 79 / 255, 1.0 * 79 / 255);
	DrawMouse(0); 
	glPopMatrix();
}

void DrawEditBar()
{
	glPushMatrix();
	glTranslatef(-3.6f, 2.5f, 0.0f);
	glScalef(0.08f, 0.08f, 0.08f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawMouse(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.6f, 1.3f, 0.0f);
	glScalef(0.08f, 0.08f, 0.08f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawMouse(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.6f,-0.05f, 0.0f);
	glScalef(0.5f, 1.0f, 0.5f);
	DrawBattery();
	glPopMatrix();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(-3.2f, 2.0f, 0.0f);
	glVertex3f(-4.0f, 2.0f, 0.1f);

	glVertex3f(-3.2f, 0.8f, 0.0f);
	glVertex3f(-4.0f, 0.8f, 0.1f);

	glVertex3f(-3.2f, -0.3f, 0.0f);
	glVertex3f(-4.0f, -0.3f, 0.1f);
	glEnd();
}

void reshape(int width, int height)
{
	if (height == 0)										
	{
		height = 1;										
	}

	gHeight = height;
	gWidth = width;

	/*glViewport(0, 0, width, height);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									

	float whRatio = (GLfloat)width / (GLfloat)height;
	gluPerspective(45, whRatio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);		*/					
}

void idle()
{
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; }

	case '+':
	{
		
		break;
	}
	case '-':
	{
		
		break;
	}
	default: break;
	}
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("%d %d\n", x, y);
		if (x <= 110)
		{
			if (y >= 0 && y <= 86)
			{
				bMouseBase = true;
			}
			else if (y <= 175)
			{
				bMouseHead = true;
			}
			else if (y <= 252)
			{
				bBattery = true;
			}
		}
	}
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();													

	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	glViewport(80, 0, gWidth - 80, gHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float whRatio = (GLfloat)gWidth / (GLfloat)gHeight;
	gluPerspective(45, whRatio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);
	
	glTranslatef(0.8f, 0.0f, 0.0f);
	glScalef(0.4, 0.4, 0.4);
	DrawScene();
	glPopMatrix();

	glPushMatrix();
	glViewport(0, 0, gWidth, gHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//whRatio = (GLfloat)gWidth / (GLfloat)gHeight;
	//gluPerspective(45, whRatio, 1, 1000);
	glOrtho(-3, 3, -3, 3, -100, 100);
	glMatrixMode(GL_MODELVIEW);

	gluLookAt(eye[0], eye[1], eye[2],
		-1.0f, 0.0f, 0.0f,
		0, 1, 0);

	DrawEditBar();
	
	glPopMatrix();

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 480);
	int windowHandle = glutCreateWindow("Mouse Assembly Workshop");

	GenHoleList();
	init();
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	glutMouseFunc(Mouse);

	glutMainLoop();
	return 0;
}
