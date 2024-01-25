#include "mouse.h"

//const float g_lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
Vec3 MousePlate::mouseHeadPos[4] = { Vec3{ 2.5f, -3.0f, -2.5f }, Vec3{ 4.0f, -3.0f, -2.5f }, Vec3{ 5.5f, -3.0f, -2.5f }, Vec3{ 7.0f, -3.0f, -2.5f } };
Vec3 MousePlate::mouseBasePos[4] = { Vec3{ -3.0f, -3.0f, 4.0f }, Vec3{ -4.5f, -3.0f, 4.0f }, Vec3{ -6.0f, -3.0f, 4.0f }, Vec3{ -7.5f, -3.0f, 4.0f } };

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
		Material::SetMaterial(Material::MouseHead);
		head.draw_obj();
	}if (type == BASE) {
		Material::SetMaterial(Material::MouseBase);
		base.draw_obj();
	}if (type == WHOLE) {
		Material::SetMaterial(Material::MouseHead);
		head.draw_obj();
		Material::SetMaterial(Material::MouseBase);
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

void MousePlate::DrawMouseHeads() const {
	for (auto& head : mouse_) {
		head.Draw();
	}
}

void MousePlate::FillMouseHeads() {
	/*mouse_.emplace_back(Vec3{ 2.5f, -3.0f, -2.5f }, HEAD);
	mouse_.emplace_back(Vec3{ 4.0f, -3.0f, -2.5f }, HEAD);
	mouse_.emplace_back(Vec3{ 5.5f, -3.0f, -2.5f }, HEAD);
	mouse_.emplace_back(Vec3{ 7.0f, -3.0f, -2.5f }, HEAD);*/
	mouse_.emplace_back(mouseHeadPos[mouse_.size()], HEAD);
	empty_ = false;
}

void MousePlate::DrawMouseBases() const {
	for (auto& head : mouse_) {
		head.Draw();
	}
}

void MousePlate::FillMouseBases() {
	/*mouse_.emplace_back(Vec3{ -3.0f, -3.0f, 4.0f }, BASE);
	mouse_.emplace_back(Vec3{ -4.5f, -3.0f, 4.0f }, BASE);
	mouse_.emplace_back(Vec3{ -6.0f, -3.0f, 4.0f }, BASE);
	mouse_.emplace_back(Vec3{ -7.5f, -3.0f, 4.0f }, BASE);*/
	mouse_.emplace_back(mouseBasePos[mouse_.size()], BASE);
	empty_ = false;
}

void MousePlate::DrawMouses() const {
	for (auto& head : mouse_) {
		head.Draw();
	}
}

void MousePlate::DrawPlates(std::vector<MousePlate>& plates) {
	glPushMatrix();

	for (auto& plate : plates) {
		plate.DrawMousePlates();
	}

	glPopMatrix();
}

void MousePlate::FillPlates(std::vector<MousePlate>& plates) {
	plates.emplace_back(Vec3{ -5.0f, -3.0f, 4.0f });
	plates.emplace_back(Vec3{ 5.0f, -3.0f, 4.0f });
	plates.emplace_back(Vec3{ 5.0f, -3.0f, -3.0f });
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