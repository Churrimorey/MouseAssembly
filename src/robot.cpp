#include "robot.h"

void DrawRobotBase()
{
	glPushMatrix();
	GLUquadricObj* cone = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glScalef(0.6f, 0.6f, 0.6f);
	gluCylinder(cone, 2.0, 1.3, 0.8, 10, 10);
	DrawCircle(0.0f, 0.0f, 0.8f, 1.3f, 10);
	DrawCircle(0.0f, 0.0f, 0.0f, 2.0f, 10);
	gluDeleteQuadric(cone);

	glPopMatrix();
}

void DrawRobotArm()
{
	GLfloat cone_mat[] =
	{ 0.f, .5f, 1.f, 1.f };

	glPushMatrix();
	GLUquadricObj* cylinder = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cone_mat);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glScalef(0.6f, 0.6f, 0.6f);
	gluCylinder(cylinder, 1.3, 1.3, 7.0, 10, 10);
	DrawCircle(0.0f, 0.0f, 7.0f, 1.3f, 10);
	DrawCircle(0.0f, 0.0f, 0.0f, 1.3f, 10);
	gluDeleteQuadric(cylinder);
	glPopMatrix();
}

void DrawRobotFinger()
{
	glPushMatrix();
	glColor3f(1.0 * 139 / 255, 1.0 * 139 / 255, 1.0 * 122 / 255);
	glScalef(0.3f, 1.2f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void DrawLeftRobot()
{
	glPushMatrix();

	glTranslatef(-6.5f, -2.5f, 1.0f);
	DrawRobotBase();

	glTranslatef(0.0f, 0.8f, 0.0f);
	glRotatef(-23.0f, 0.0f, 0.0f, 1.0f);
	DrawRobotArm();

	glTranslatef(0.0f, 4.5f, 0.0f);
	glRotatef(-123.0f, 0.0f, 0.0f, 1.0f);
	DrawRobotArm();

	glPushMatrix();
	glTranslatef(0.0f, 4.8f, -0.6f);
	DrawRobotFinger();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 4.8f, 0.6f);
	DrawRobotFinger();
	glPopMatrix();

	glPopMatrix();
}

void DrawRightRobot()
{
	glPushMatrix();

	glTranslatef(6.5f, -2.5f, 1.0f);
	DrawRobotBase();

	glTranslatef(0.0f, 0.8f, 0.0f);
	glRotatef(23.0f, 0.0f, 0.0f, 1.0f);
	DrawRobotArm();

	glTranslatef(0.0f, 4.5f, 0.0f);
	glRotatef(123.0f, 0.0f, 0.0f, 1.0f);
	DrawRobotArm();

	glPushMatrix();
	glTranslatef(0.0f, 4.8f, -0.6f);
	DrawRobotFinger();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 4.8f, 0.6f);
	DrawRobotFinger();
	glPopMatrix();

	glPopMatrix();
}
