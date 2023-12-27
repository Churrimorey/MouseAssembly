#include "robot.h"
#include "material.h"

Mouse Robot::left_mouse_;
Mouse Robot::right_mouse_;
Battery Robot::left_battery_;
Battery Robot::right_battery_;

void RobotBase::Draw() const {
	const auto& position = GetPosition();
	glTranslatef(position.GetX(), position.GetY(), position.GetZ());

	glPushMatrix();
	Material::SetMaterial(Material::Base);
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

	glPushMatrix();
	Material::SetMaterial(Material::Arm);
	GLUquadricObj* cylinder = gluNewQuadric();
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
	Material::SetMaterial(Material::Finger);
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
	Robot* root = new RobotBase({ -6.5f, -2.5f, 1.0f }, 0.0f);
	auto robot = root;
	robot->SetNext(new RobotArm({ 0.0f, 0.8f, 0.0f }, -23.0f));
	robot = robot->GetNext();
	static_cast<RobotArm*>(robot)->SetPrev(root);
	auto temp = robot;
	robot->SetNext(new RobotArm({ 0.0f, 4.5f, 0.0f }, -123.0));
	robot = robot->GetNext();
	static_cast<RobotArm*>(robot)->SetPrev(temp);
	robot->SetNext(new RobotFinger({ 0.0f, 4.8f, -0.6f }));
	robot = robot->GetNext();
	robot->SetNext(new RobotFinger({ 0.0f, 4.8f, 0.6f }));
	return root;
}

Robot* Robot::CreateRightRobot() {
	Robot* root = new RobotBase({ 6.5f, -2.5f, 1.0f }, 0.0f);
	auto robot = root;
	robot->SetNext(new RobotArm({ 0.0f, 0.8f, 0.0f }, 23.0f));
	robot = robot->GetNext();
	static_cast<RobotArm*>(robot)->SetPrev(root);
	auto temp = robot;
	robot->SetNext(new RobotArm({ 0.0f, 4.5f, 0.0f }, 123.0));
	robot = robot->GetNext();
	static_cast<RobotArm*>(robot)->SetPrev(temp);
	robot->SetNext(new RobotFinger({ 0.0f, 4.8f, -0.6f }));
	robot = robot->GetNext();
	robot->SetNext(new RobotFinger({ 0.0f, 4.8f, 0.6f }));
	return root;
}

static void Transpose(GLfloat* matrix) {
	// 转置
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < i; j++) {
			std::swap(matrix[i * 4 + j], matrix[j * 4 + i]);
		}
	}
}

/**
 * @brief update the left robot arm's position, move to the detination
 * 
 * @param robot - the left robot
 * @param destination - the Mouse we want to catch (chosen in the redraw(main.cpp))
 * @param elapsed_time - the elapsed time (using the clock)
 * @return true - We arrive at the destination
 * @return false - We havn't arrived at the destination
 */
bool Robot::UpdateLeftPositionToMouse(Robot &robot, const Mouse& destination, double elapsed_time) {
	Vec3 diff = destination.GetPosition() - robot.GetPosition();
	
	RobotBase* base = static_cast<RobotBase*>(&robot);
	RobotArm* arm1 = static_cast<RobotArm *>(robot.GetNext());
	RobotArm* arm2 = static_cast<RobotArm*>(arm1->GetNext());

	Vec3 finger_position = arm2->GetLocalPosition();
	// 计算下一步位置
	auto delta = (diff - finger_position).Unit() * elapsed_time;
	finger_position += delta;
	UpdateLeftPositionToMouse(static_cast<RobotBase&>(robot), *arm1, *arm2, finger_position);
	auto len = (arm2->GetLocalPosition() - destination.GetPosition()).Length();
	if (len < 1.25) {
		left_mouse_ = destination;
		return true;
	}
	return false;
}

/**
 * @brief update the left robot arm's position, move to the detination
 * 
 * @param robot - the left robot
 * @param destination - the Battery we want to catch (chosen in the redraw(main.cpp))
 * @param elapsed_time - the elapsed time (using the clock)
 * @return true - We arrive at the destination
 * @return false - We havn't arrived at the destination
 */
bool Robot::UpdateLeftPositionToBattery(Robot& robot, const Battery& destination, double elapsed_time) {
	Vec3 diff = destination.GetPosition() + Vec3{ 0.0f, -3.0f, 0.0f } - robot.GetPosition();

	RobotBase* base = static_cast<RobotBase*>(&robot);
	RobotArm* arm1 = static_cast<RobotArm*>(robot.GetNext());
	RobotArm* arm2 = static_cast<RobotArm*>(arm1->GetNext());

	Vec3 finger_position = arm2->GetLocalPosition();
	// 计算下一步位置
	auto delta = (diff - finger_position).Unit() * elapsed_time * 10;
	finger_position += delta;
	UpdateLeftPositionToBattery(static_cast<RobotBase&>(robot), *arm1, *arm2, finger_position);
	static_cast<RobotBase&>(robot).SetRotate(static_cast<RobotBase&>(robot).GetRotate() + 0.1);
	auto len = (arm2->GetLocalPosition() - destination.GetPosition()).Length();
	if (len < 1.20) {
		left_battery_ = destination;
		return true;
	}
	return false;
}

/**
 * @brief update the right robot arm's position, move to the detination
 * 
 * @param robot - the right robot
 * @param destination - the Mouse we want to catch (chosen in the redraw(main.cpp))
 * @param elapsed_time - the elapsed time (using the clock)
 * @return true - We arrive at the destination
 * @return false - We havn't arrived at the destination
 */
bool Robot::UpdateRightPositionToMouse(Robot& robot, const Mouse& destination, double elapsed_time) {
	Vec3 diff = destination.GetPosition() - robot.GetPosition();

	RobotBase* base = static_cast<RobotBase*>(&robot);
	RobotArm* arm1 = static_cast<RobotArm*>(robot.GetNext());
	RobotArm* arm2 = static_cast<RobotArm*>(arm1->GetNext());

	Vec3 finger_position = arm2->GetLocalPosition();
	// 计算下一步位置
	auto delta = (diff - finger_position).Unit() * elapsed_time;
	finger_position += delta;
	UpdateRightPositionToMouse(static_cast<RobotBase&>(robot), *arm1, *arm2, finger_position);
	auto len = (arm2->GetLocalPosition() - destination.GetPosition()).Length();
	if (len < 1.5) {
		left_mouse_ = destination;
		return true;
	}
	return false;
}

/**
 * @brief update the right robot arm's position, move to the detination
 * 
 * @param robot - the right robot
 * @param destination - the Battery we want to catch (chosen in the redraw(main.cpp))
 * @param elapsed_time - the elapsed time (using the clock)
 * @return true - We arrive at the destination
 * @return false - We havn't arrived at the destination
 */
bool Robot::UpdateRightPositionToBattery(Robot& robot, const Battery& destination, double elapsed_time) {
	Vec3 diff = destination.GetPosition() + Vec3{ 0.0f, -3.0f, 0.0f } - robot.GetPosition();

	RobotBase* base = static_cast<RobotBase*>(&robot);
	RobotArm* arm1 = static_cast<RobotArm*>(robot.GetNext());
	RobotArm* arm2 = static_cast<RobotArm*>(arm1->GetNext());

	Vec3 finger_position = arm2->GetLocalPosition();
	// 计算下一步位置
	auto delta = (diff - finger_position).Unit() * elapsed_time * 10;
	finger_position += delta;
	UpdateRightPositionToBattery(static_cast<RobotBase&>(robot), *arm1, *arm2, finger_position);
	static_cast<RobotBase&>(robot).SetRotate(static_cast<RobotBase&>(robot).GetRotate() + 0.1);
	auto len = (arm2->GetLocalPosition() - destination.GetPosition()).Length();
	if (len < 1.20) {
		left_battery_ = destination;
		return true;
	}
	return false;
}

/** 
 * UpdatePosition - 更新机械臂位置
 * 
 */
void Robot::UpdateLeftPositionToMouse(RobotBase& base, RobotArm& arm1, RobotArm& arm2, const Vec3& position) {
	auto len = position.Length();
	base.SetRotate(0.0f);
	auto non_rotate = arm2.GetLocalPosition();
	auto cosx = position.CrossXZ(non_rotate) / non_rotate.LengthXZ() / position.LengthXZ();
	auto angle = -acosf(cosx) / PI * 180.0;
	base.SetRotate(angle);

	GLfloat matrix[16];

	// 得到若第二个机械臂不旋转所在的位置
	arm2.SetRotate(0);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(base.GetPosition().GetX(), base.GetPosition().GetY(), base.GetPosition().GetZ());
	glRotatef(base.GetRotate(), 0.0f, 1.0f, 0.0f);
	glTranslatef(arm1.GetPosition().GetX(), arm1.GetPosition().GetY(), arm1.GetPosition().GetZ());
	glRotatef(arm1.GetRotate(), 0.0f, 0.0f, 1.0f);
	glTranslatef(arm2.GetPosition().GetX(), arm2.GetPosition().GetY(), arm2.GetPosition().GetZ());
	glRotatef(arm2.GetRotate(), 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	Transpose(matrix);
	auto arm2_non_rotate = Vec3{ 0,0,0 } * matrix;
	glPopMatrix();
	cosx = arm2_non_rotate.Cross(position) / arm2_non_rotate.Length() / len;
	angle = acosf(cosx) / PI * 180.0 - 180.0;
	arm2.SetRotate(angle);
	/*auto arm_base = arm2.GetLocalPosition() - base.GetPosition();
	angle = acosf(arm_base.Cross(position) / arm_base.Length() / len);
	arm1.SetRotate(arm1.GetRotate() + angle);*/
	/*arm1.SetRotate(0);
	auto arm1_non_rotate = arm2.GetLocalPosition();
	angle = -acosf(arm1_non_rotate.Cross(position) / arm1_non_rotate.Length() / len) / PI * 180.0 / 2;
	arm1.SetRotate(angle);*/
}

void Robot::UpdateLeftPositionToBattery(RobotBase& base, RobotArm& arm1, RobotArm& arm2, const Vec3& position) {
	auto len = position.Length();
	base.SetRotate(0.0f);
	auto non_rotate = arm2.GetLocalPosition();
	auto cosx = position.CrossXZ(non_rotate) / non_rotate.LengthXZ() / position.LengthXZ();
	auto angle = acosf(cosx) / PI * 180.0 / 4;
	base.SetRotate(angle);

	GLfloat matrix[16];

	// 得到若第二个机械臂不旋转所在的位置
	arm2.SetRotate(0);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(base.GetPosition().GetX(), base.GetPosition().GetY(), base.GetPosition().GetZ());
	glRotatef(base.GetRotate(), 0.0f, 1.0f, 0.0f);
	glTranslatef(arm1.GetPosition().GetX(), arm1.GetPosition().GetY(), arm1.GetPosition().GetZ());
	glRotatef(arm1.GetRotate(), 0.0f, 0.0f, 1.0f);
	glTranslatef(arm2.GetPosition().GetX(), arm2.GetPosition().GetY(), arm2.GetPosition().GetZ());
	glRotatef(arm2.GetRotate(), 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	Transpose(matrix);
	auto arm2_non_rotate = Vec3{ 0,0,0 } *matrix;
	glPopMatrix();
	cosx = arm2_non_rotate.Cross(position) / arm2_non_rotate.Length() / len;
	angle = acosf(cosx) / PI * 180.0 - 180.0;
	arm2.SetRotate(angle);
}

void Robot::UpdateRightPositionToMouse(RobotBase& base, RobotArm& arm1, RobotArm& arm2, const Vec3& position) {
	auto len = position.Length();
	base.SetRotate(0.0f);
	auto non_rotate = arm2.GetLocalPosition();
	auto cosx = position.CrossXZ(non_rotate) / non_rotate.LengthXZ() / position.LengthXZ();
	auto angle = -acosf(cosx) / PI * 180.0;
	base.SetRotate(angle);

	GLfloat matrix[16];

	// 得到若第二个机械臂不旋转所在的位置
	arm2.SetRotate(0);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(base.GetPosition().GetX(), base.GetPosition().GetY(), base.GetPosition().GetZ());
	glRotatef(base.GetRotate(), 0.0f, 1.0f, 0.0f);
	glTranslatef(arm1.GetPosition().GetX(), arm1.GetPosition().GetY(), arm1.GetPosition().GetZ());
	glRotatef(arm1.GetRotate(), 0.0f, 0.0f, 1.0f);
	glTranslatef(arm2.GetPosition().GetX(), arm2.GetPosition().GetY(), arm2.GetPosition().GetZ());
	glRotatef(arm2.GetRotate(), 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	Transpose(matrix);
	auto arm2_non_rotate = Vec3{ 0,0,0 } *matrix;
	glPopMatrix();
	cosx = arm2_non_rotate.Cross(position) / arm2_non_rotate.Length() / len;
	angle = 180.0 - acosf(cosx) / PI * 180.0;
	arm2.SetRotate(angle);
}

void Robot::UpdateRightPositionToBattery(RobotBase& base, RobotArm& arm1, RobotArm& arm2, const Vec3& position) {
	auto len = position.Length();
	base.SetRotate(0.0f);
	auto non_rotate = arm2.GetLocalPosition();
	auto cosx = position.CrossXZ(non_rotate) / non_rotate.LengthXZ() / position.LengthXZ();
	auto angle = -acosf(cosx) / PI * 180.0 / 4;
	base.SetRotate(angle);

	GLfloat matrix[16];

	// 得到若第二个机械臂不旋转所在的位置
	arm2.SetRotate(0);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(base.GetPosition().GetX(), base.GetPosition().GetY(), base.GetPosition().GetZ());
	glRotatef(base.GetRotate(), 0.0f, 1.0f, 0.0f);
	glTranslatef(arm1.GetPosition().GetX(), arm1.GetPosition().GetY(), arm1.GetPosition().GetZ());
	glRotatef(arm1.GetRotate(), 0.0f, 0.0f, 1.0f);
	glTranslatef(arm2.GetPosition().GetX(), arm2.GetPosition().GetY(), arm2.GetPosition().GetZ());
	glRotatef(arm2.GetRotate(), 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	Transpose(matrix);
	auto arm2_non_rotate = Vec3{ 0,0,0 } *matrix;
	glPopMatrix();
	cosx = arm2_non_rotate.Cross(position) / arm2_non_rotate.Length() / len;
	angle = 180.0 - acosf(cosx) / PI * 180.0;
	arm2.SetRotate(angle);
}

Battery& Robot::GetClosestBattery(Robot& robot, std::vector<Battery>& batteries) {
	RobotBase* base = static_cast<RobotBase*>(&robot);
	RobotArm* arm1 = static_cast<RobotArm*>(robot.GetNext());
	RobotArm* arm2 = static_cast<RobotArm*>(arm1->GetNext());

	auto local_position = arm2->GetLocalPosition();
	double min_len = 0x7fffffff;
	int min_index = 0;
	for (int i = 0; i < batteries.size(); i++) {
		auto len = (batteries[i].GetPosition() - local_position).Length();
		if (len < min_len) {
			min_index = i;
			min_len = len;
		}
	}
	return batteries[min_index];
}

Vec3 RobotArm::GetLocalPosition() const {
	auto arm1 = static_cast<RobotArm*>(prev_);
	auto base = static_cast<RobotBase*>(arm1->prev_);
	GLfloat matrix[16];
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(base->GetPosition().GetX(), base->GetPosition().GetY(), base->GetPosition().GetZ());
	glRotatef(base->GetRotate(), 0.0f, 1.0f, 0.0f);
	glTranslatef(arm1->GetPosition().GetX(), arm1->GetPosition().GetY(), arm1->GetPosition().GetZ());
	glRotatef(arm1->GetRotate(), 0.0f, 0.0f, 1.0f);
	glTranslatef(GetPosition().GetX(), GetPosition().GetY(), GetPosition().GetZ());
	glRotatef(GetRotate(), 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 4.8f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	Transpose(matrix);
	auto local_position = Vec3{ 0,0,0 } *matrix;
	glPopMatrix();
	return local_position;
}
