#include "Animation.h"

void Animation::Update() {
	clock_.update();
	auto time = clock_.GetElapsedTime();
	switch (state_) {
	case State::LEFTGETTOMOUSEHEAD: {
		if (!plates_[2].GetMouses().empty() && plates_[1].GetMouses().size() < 4) {
			UpdateLeftHead(std::min(timer_, time));
			timer_ -= time;
			if (timer_ <= 0) {
				state_ = State::LEFTTOWORKPLACE;
				Robot::has_left_mouse_ = true;
				plates_[2].GetMouses().erase(plates_[2].GetMouses().begin());
				timer_ = 1.0;
				left_base_rotate_ = static_cast<RobotBase*>(left_robot_.get())->GetRotate() + 15.0;
				left_arm1_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext())->GetRotate() + 35.0;
				left_arm2_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext()->GetNext())->GetRotate() + 80.0;
			}
		}
		else {
			bAnim = false;
		}
		break;
	}
	case State::LEFTTOWORKPLACE: {
		UpdateLeftHead(std::min(timer_, time));
		timer_ -= time;
		if (timer_ <= 0) {
			state_ = State::LEFTGETBATTERY;
			timer_ = 1.0;
			left_base_rotate_ = static_cast<RobotBase*>(left_robot_.get())->GetRotate() + 5.0;
			left_arm1_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext())->GetRotate() + 30.0;
			left_arm2_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext()->GetNext())->GetRotate() + 110.0;
			Robot::has_left_mouse_ = false;
			Robot::left_mouse_.SetPosition(Vec3{ -0.0f, -1.3f, 2.0f });
		}
		break;
	}
	case State::LEFTGETBATTERY: {
		Robot::left_mouse_.Draw();
		if (!battries_.empty()) {
			UpdateLeftHead(std::min(timer_, time));
			timer_ -= time;
			if (timer_ <= 0) {
				state_ = State::LEFTBATTERYTOWORKPLACE;
				Robot::has_left_battery_ = true;
				timer_ = 1.0;
				left_base_rotate_ = static_cast<RobotBase*>(left_robot_.get())->GetRotate() + 15.0;
				left_arm1_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext())->GetRotate() + 35.0;
				left_arm2_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext()->GetNext())->GetRotate() + 80.0;
			}
		}
		else {
			bAnim = false;
		}
		break;
	}
	case State::LEFTBATTERYTOWORKPLACE: {
		Robot::left_mouse_.Draw();
		UpdateLeftHead(std::min(timer_, time));
		timer_ -= time;
		if (timer_ <= 0) {
			state_ = State::RIGHTGETBATTERY;
			Robot::has_left_battery_ = false;
			timer_ = 1.0;
			left_base_rotate_ = static_cast<RobotBase*>(left_robot_.get())->GetRotate();
			left_arm1_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext())->GetRotate() + 23.0;
			left_arm2_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext()->GetNext())->GetRotate() + 123.0;
			right_base_rotate_ = static_cast<RobotBase*>(right_robot_.get())->GetRotate() - 5.0;
			right_arm1_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext())->GetRotate() - 30.0;
			right_arm2_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->GetRotate() - 115.0;
		}
		break;
	}
	case State::RIGHTGETBATTERY: {
		Robot::left_mouse_.Draw();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.3f, -2.7f, 0.2f);
		Robot::left_battery_.Draw();
		glPopMatrix();
		UpdateLeftHead(std::min(timer_, time));
		UpdateRightHead(std::min(timer_, time));
		timer_ -= time;
		if (timer_ <= 0) {
			state_ = State::RIGHTBATTERYTOWORKPLACE;
			Robot::has_right_battery_ = true;
			timer_ = 1.0;
			right_base_rotate_ = static_cast<RobotBase*>(right_robot_.get())->GetRotate() - 15.0;
			right_arm1_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext())->GetRotate() - 23.0;
			right_arm2_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->GetRotate() - 100.0;
		}
		break;
	}
	case State::RIGHTBATTERYTOWORKPLACE: {
		Robot::left_mouse_.Draw();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.3f, -2.7f, 0.2f);
		Robot::left_battery_.Draw();
		glPopMatrix();
		UpdateRightHead(std::min(timer_, time));
		timer_ -= time;
		if (timer_ <= 0) {
			state_ = State::RIGHTGETTOMOUSEBASE;
			Robot::has_right_battery_ = false;
			timer_ = 1.0;
			right_base_rotate_ = static_cast<RobotBase*>(right_robot_.get())->GetRotate() + 45.0;
			right_arm1_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext())->GetRotate() - 30.0;
			right_arm2_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->GetRotate() - 120.0;
		}
		break;
	}
	case State::RIGHTGETTOMOUSEBASE: {
		Robot::left_mouse_.Draw();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.3f, -2.7f, 0.2f);
		Robot::left_battery_.Draw();
		glPopMatrix();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.8f, -2.7f, 0.2f);
		Robot::right_battery_.Draw();
		glPopMatrix();
		if (!plates_[0].GetMouses().empty()) {
			UpdateRightHead(std::min(timer_, time));
			timer_ -= time;
			if (timer_ <= 0) {
				plates_[0].GetMouses().erase(plates_[0].GetMouses().begin());
				state_ = State::RIGHTTOWORKPLACE;
				Robot::has_right_mouse_ = true;
				timer_ = 1.0;
				right_base_rotate_ = static_cast<RobotBase*>(right_robot_.get())->GetRotate() - 15.0;
				right_arm1_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext())->GetRotate() - 23.0;
				right_arm2_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->GetRotate() - 100.0;
			}
		}
		else {
			bAnim = false;
		}
		break;
	}
	case State::RIGHTTOWORKPLACE: {
		Robot::left_mouse_.Draw();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.3f, -2.7f, 0.2f);
		Robot::left_battery_.Draw();
		glPopMatrix();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.8f, -2.7f, 0.2f);
		Robot::right_battery_.Draw();
		glPopMatrix();
		UpdateRightHead(std::min(timer_, time));
		timer_ -= time; 
		if (timer_ <= 0) {
			state_ = State::ASSEMBLE;
			timer_ = 1.0;
			Robot::has_right_mouse_ = false;
			Robot::has_mouse_ = true;
			right_base_rotate_ = static_cast<RobotBase*>(right_robot_.get())->GetRotate() - 45;
			right_arm1_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext())->GetRotate() - 30.0;
			right_arm2_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->GetRotate() - 123.0;
		}
		break;
	}
	case State::ASSEMBLE: {
		UpdateRightHead(std::min(timer_, time));
		timer_ -= time;
		if (timer_ <= 0) {
			Robot::mouse_.SetPosition(Vec3{ 2.0f, -3.0f, 2.5f });
			plates_[1].GetMouses().push_back(Robot::mouse_);
			state_ = State::REINITIAL;
			Robot::has_mouse_ = false;
			/*plates_[2].GetMouses().push_back({ {-9.0f, -2.5f, 4.0f}, BASE });
			plates_[0].GetMouses().push_back({ {8.5f, -2.5f, -2.5f}, HEAD });*/
			left_base_rotate_ = static_cast<RobotBase*>(left_robot_.get())->GetRotate();
			left_arm1_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext())->GetRotate() + 23.0;
			left_arm2_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext()->GetNext())->GetRotate() + 123.0;

			right_base_rotate_ = static_cast<RobotBase*>(right_robot_.get())->GetRotate();
			right_arm1_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext())->GetRotate() - 23.0;
			right_arm2_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->GetRotate() - 123.0;
			timer_ = 1.0;
		}
		break;
	}
	case State::REINITIAL: {
		UpdateLeftHead(std::min(timer_, time));
		UpdateRightHead(std::min(timer_, time));
		timer_ -= clock_.GetElapsedTime();
		for (auto& mouse : plates_[0].GetMouses()) {
			mouse.SetPosition(mouse.GetPosition() + Vec3(-1.5 * time, 0.0f, 0.0f));
		}

		for (auto& mouse : plates_[2].GetMouses()) {
			mouse.SetPosition(mouse.GetPosition() + Vec3(1.5 * time, 0.0f, 0.0f));
		}

		if(plates_[1].GetMouses().size() < 4)
		{
			for (auto& mouse : plates_[1].GetMouses()) {
			mouse.SetPosition(mouse.GetPosition() + Vec3(1.5 * time, 0.0f, 0.0f));
			}
		}
		if (timer_ <= 0) {
			state_ = State::LEFTGETTOMOUSEHEAD;
			Robot::left_mouse_.SetPosition({ 0.0f, -0.2f, -1.2f });
			Robot::right_mouse_.SetPosition({ 0.0f, 0.0f, -1.2f });
			Robot::left_battery_.SetPosition({ 0.0f, 0.0f, -1.0f });
			Robot::right_battery_.SetPosition({ 0.0f, 0.0f, -1.0f });
			Robot::mouse_.SetPosition({ 0.0f, 0.0f, -1.2f });
			timer_ = 1;
			left_base_rotate_ = 36.0f;
			left_arm1_rotate_ =  0.0f;
			left_arm2_rotate_ = 0.0f;
			bAnim = false;
			plates_[0].UpdateEmpty();
			plates_[2].UpdateEmpty();
		}
	}
	}
}

void Animation::UpdateLeftHead(double time) {
	static_cast<RobotBase*>(left_robot_.get())->SetRotate(static_cast<RobotBase*>(left_robot_.get())->GetRotate() - left_base_rotate_ * time);
	static_cast<RobotArm*>(left_robot_->GetNext())->SetRotate(static_cast<RobotArm*>(left_robot_->GetNext())->GetRotate() - left_arm1_rotate_ * time);
	static_cast<RobotArm*>(left_robot_->GetNext()->GetNext())->SetRotate(static_cast<RobotArm*>(left_robot_->GetNext()->GetNext())->GetRotate() - left_arm2_rotate_ * time);
}

void Animation::UpdateRightHead(double time) {
	static_cast<RobotBase*>(right_robot_.get())->SetRotate(static_cast<RobotBase*>(right_robot_.get())->GetRotate() - right_base_rotate_ * time);
	static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->SetRotate(static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->GetRotate() - right_arm2_rotate_ * time);

	/*Robot::left_mouse_.SetPosition( {0.0f, -0.2f, -1.2f} );
	Robot::right_mouse_.SetPosition( {0.0f, 0.0f, -1.2f} );
	Robot::left_battery_.SetPosition( {0.0f, 0.0f, -1.0f} );
	Robot::right_battery_.SetPosition( {0.0f, 0.0f, -1.0f} );
	Robot::mouse_.SetPosition( {0.0f, 0.0f, -1.2f} );*/
}