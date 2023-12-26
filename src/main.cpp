#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gl/glut.h>
#include <iostream>
#include <memory>
#include <vector>
#include "mouse.h"
#include "utils.h"
#include "drawings.h"
#include "robot.h"
#include "light.h"
#include "material.h"
#include "clock.h"
#include "menu.h"

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
std::unique_ptr<Robot> left_robot;    // ���е��
std::unique_ptr<Robot> right_robot;   // �һ�е��
std::vector<MousePlate> plates;
std::vector<Battery> battries;
Clock myclock{ true };
Menu menu(1);

Mouse head = Mouse(Vec3(0, 0, 0), HEAD);
Mouse base = Mouse(Vec3(0, 0, 0), BASE);

void init()
{
	#ifdef MACOS
	GLint buf[1], sbuf[1];
	glGetIntegerv(GL_SAMPLE_BUFFERS_ARB, buf);
	glGetIntegerv(GL_SAMPLES_ARB, sbuf);
	glEnable(GL_MULTISAMPLE_ARB);
	#endif

	glGenTextures(2, texture);
	texload(0, (char*)"table.bmp");
	texload(1, (char*)"battery.bmp");
	left_robot.reset(Robot::CreateLeftRobot());
	right_robot.reset(Robot::CreateRightRobot());

	Light::InitLight(menu);
	Material::InitMaterial();
}

// 获取并打印模型视图矩阵
void printModelViewMatrix() {
	GLfloat modelViewMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
	std::cout << "Model View Matrix:" << std::endl;
	for (int i = 0; i < 16; i++) {
		std::cout << modelViewMatrix[i] << " ";
		if ((i + 1) % 4 == 0) std::cout << std::endl;
	}
}

void DrawScene()
{
	Light::FlushLight();

	DrawTable();

	left_robot->DrawRobot();

	right_robot->DrawRobot();

	Material::SetMaterial(Material::Desk);
	DrawDesk();

	Material::SetMaterial(Material::Box);
	DrawBox();

	if (bBattery)
	{
		Battery::DrawBatterys(battries);
	}
	Material::SetMaterial(Material::Plate);
	MousePlate::DrawPlates(plates);

	if (bMouseHead)
	{
		Material::SetMaterial(Material::MouseHead);
		plates[0].DrawMouseHeads();
	}

	if (bMouseBase)
	{
		Material::SetMaterial(Material::MouseBase);
		plates[2].DrawMouseBases();
	}

	glPushMatrix();
	printModelViewMatrix();
	//glLoadIdentity();
	glTranslatef(2.5f, -2.5f, 5.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.50f, 0.50f, 0.50f);
	Material::SetMaterial(Material::MouseHead);
	head.DrawMouse(HEAD);
	Material::SetMaterial(Material::MouseBase);
	base.DrawMouse(BASE);
	glPopMatrix();
}

void DrawEditBar()
{
		glPushMatrix();
		glTranslatef(-3.6f, 2.5f, 0.0f);
		glScalef(0.8f, 0.8f, 0.8f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		head.DrawMouse(HEAD);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-3.6f, 1.3f, 0.0f);
		glScalef(0.8f, 0.8f, 0.8f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		base.DrawMouse(BASE);
		glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.6f, -0.05f, 0.0f);
	glScalef(0.5f, 1.0f, 0.5f);
	Battery::DrawBattery(0, 0, 0);
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

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (menu.Hit(x, gHeight - y))
			return;
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
	float whRatio = (GLfloat)(gWidth - 80) / (GLfloat)gHeight;
	gluPerspective(45, whRatio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);

	glTranslatef(0.8f, 0.0f, 0.0f);
	glScalef(0.4, 0.4, 0.4);
	DrawScene();
	myclock.update();
	if (!plates[2].GetMouses().empty()) {
		if (Robot::UpdateLeftPositionToMouse(*left_robot, plates[2].GetMouses()[0], myclock.GetElapsedTime())) {
			std::cout << true << std::endl;
			plates[2].GetMouses().erase(plates[2].GetMouses().begin());
		}
	}
	if (!plates[0].GetMouses().empty()) {
		if (Robot::UpdateRightPositionToMouse(*right_robot, plates[0].GetMouses()[0], myclock.GetElapsedTime())) {
			std::cout << true << std::endl;
			plates[0].GetMouses().erase(plates[0].GetMouses().begin());
		}
	}
	if (!battries.empty()) {
		if (Robot::UpdateLeftPositionToBattery(*left_robot, Robot::GetClosestBattery(*left_robot, battries), myclock.GetElapsedTime())) {
			std::cout << true << std::endl;
		}
		if (Robot::UpdateRightPositionToBattery(*right_robot, Robot::GetClosestBattery(*right_robot, battries), myclock.GetElapsedTime())) {
			std::cout << true << std::endl;
		}
	}
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

	glPushMatrix();
	glViewport(0, 0, gWidth, gHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, gWidth, 0, gHeight);
	glMatrixMode(GL_MODELVIEW);
	menu.Draw(0, 0);
	glPopMatrix();

	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 480);
	int windowHandle = glutCreateWindow("Mouse Assembly Workshop");

	GenHoleList();
	init();
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);

	glutMainLoop();
	return 0;
}
