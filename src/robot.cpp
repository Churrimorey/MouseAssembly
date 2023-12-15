#include "robot.h"
#include <gl/gl.h>

void RobotBase::Draw() const {
	const auto& position = GetPosition();
	glTranslatef(position.GetX(), position.GetY(), position.GetZ());

	glPushMatrix();
	GLUquadricObj* cone = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glScalef(0.6f, 0.6f, 0.6f);
	gluCylinder(cone, 2.0, 1.3, 0.8, 10, 10);
	DrawCircle(0.0f, 0.0f, 0.8f, 1.3f, 10);
	DrawCircle(0.0f, 0.0f, 0.0f, 2.0f, 10);
	gluDeleteQuadric(cone);

	glPopMatrix();
	if (GetNext() != nullptr) {
		GetNext()->Draw();
	}
}

void RobotArm::Draw() const {
	const auto& position = GetPosition();
	glTranslatef(position.GetX(), position.GetY(), position.GetZ());
	glRotatef(rotate_, 0.0f, 0.0f, 1.0f);

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
	if (GetNext() != nullptr) {
		GetNext()->Draw();
	}
}

void RobotFinger::Draw() const {
	glPushMatrix();
	const auto& position = GetPosition();
	glTranslatef(position.GetX(), position.GetY(), position.GetZ());
	glPushMatrix();
	glColor3f(1.0 * 139 / 255, 1.0 * 139 / 255, 1.0 * 122 / 255);
	glScalef(0.3f, 1.2f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();
	glPopMatrix();
	if (GetNext() != nullptr) {
		GetNext()->Draw();
	}
}

void Robot::DrawRobot() const {
	glPushMatrix();
	Draw();
	glPopMatrix();
}

Robot* Robot::CreateLeftRobot() {
	Robot* root = new RobotBase({ -6.5f, -2.5f, 1.0f });
	auto robot = root;
	robot->SetNext(new RobotArm({ 0.0f, 0.8f, 0.0f }, -23.0f));
	robot = robot->GetNext();
	robot->SetNext(new RobotArm({ 0.0f, 4.5f, 0.0f }, -123.0));
	robot = robot->GetNext();
	robot->SetNext(new RobotFinger({ 0.0f, 4.8f, -0.6f }));
	robot = robot->GetNext();
	robot->SetNext(new RobotFinger({ 0.0f, 4.8f, 0.6f }));
	return root;
}

Robot* Robot::CreateRightRobot() {
	Robot* root = new RobotBase({ 6.5f, -2.5f, 1.0f });
	auto robot = root;
	robot->SetNext(new RobotArm({ 0.0f, 0.8f, 0.0f }, 23.0f));
	robot = robot->GetNext();
	robot->SetNext(new RobotArm({ 0.0f, 4.5f, 0.0f }, 123.0));
	robot = robot->GetNext();
	robot->SetNext(new RobotFinger({ 0.0f, 4.8f, -0.6f }));
	robot = robot->GetNext();
	robot->SetNext(new RobotFinger({ 0.0f, 4.8f, 0.6f }));
	return root;
}
