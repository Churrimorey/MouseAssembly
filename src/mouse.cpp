#include "mouse.h"

//const float g_lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f };

Mouse::Mouse(const Vec3& position, Type type) : position_(position), type_(type) {
	if (type == HEAD) {
		char filename[] = "./tip.obj";
		head.open_obj(filename);
	}
	else if (type == BASE) {
		char filename[] = "./base.obj";
		base.open_obj(filename);
	}
	else {
		head.open_obj("./tip.obj");
		base.open_obj("./base.obj");
	}
}

void Mouse::DrawMouse(Type type)const
{
	if (type == HEAD) {
		head.draw_obj();
	}if (type == BASE) {
		base.draw_obj();
	}if (type == WHOLE) {
		head.draw_obj();
		base.draw_obj();
	}
}

void MousePlate::DrawMousePlates() const {
	glPushMatrix();
	glTranslatef(position_.GetX(), position_.GetY(), position_.GetZ());
	glScalef(6.0f, 0.1, 3.0f);
	glutSolidCube(1);
	glPopMatrix();
}

void Mouse::Draw() const {
	glPushMatrix();
	glTranslatef(position_.GetX()-0.3, position_.GetY(), position_.GetZ()+1.5);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.6f, 0.6f, 0.6f);
	DrawMouse(type_);
	glPopMatrix();
}

void MousePlate::DrawMouseHeads() {
	mouse_.emplace_back(Vec3{ 2.5f, -2.5f, -2.5f }, HEAD);
	mouse_.emplace_back(Vec3{ 4.0f, -2.5f, -2.5f }, HEAD);
	mouse_.emplace_back(Vec3{ 5.5f, -2.5f, -2.5f }, HEAD);
	mouse_.emplace_back(Vec3{ 7.0f, -2.5f, -2.5f }, HEAD);
	
	for (auto& head : mouse_) {
		head.Draw();
	}
}

void MousePlate::DrawMouseBases() {
	mouse_.emplace_back(Vec3{ -3.0f, -2.5f, 4.0f }, BASE);
	mouse_.emplace_back(Vec3{ -4.5f, -2.5f, 4.0f }, BASE);
	mouse_.emplace_back(Vec3{ -6.0f, -2.5f, 4.0f }, BASE);
	mouse_.emplace_back(Vec3{ -7.5f, -2.5f, 4.0f }, BASE);

	for (auto& head : mouse_) {
		head.Draw();
	}
}

void MousePlate::DrawPlates(std::vector<MousePlate>& plates) {
	glPushMatrix();

	plates.emplace_back(Vec3{ -5.0f, -3.0f, 4.0f });
	plates.emplace_back(Vec3{ 5.0f, -3.0f, 4.0f });
	plates.emplace_back(Vec3{ 5.0f, -3.0f, -3.0f });
	for (auto& plate : plates) {
		plate.DrawMousePlates();
	}

	glPopMatrix();
}

//glClearColor(0.2, 0.55, 1.0, 1);
//
//glDisable(GL_CULL_FACE);
//
//glEnable(GL_DEPTH_TEST);
//glDepthFunc(GL_LESS);
//
//glEnable(GL_LIGHT0);
//glEnable(GL_LIGHTING);
//glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos);