#include "Animation.h"

void Animation::Update() {
	clock_.update();
	switch (state_) {
	case State::LEFTGETTOMOUSEHEAD: {
		if (!plates_[2].GetMouses().empty()) {
			if (Robot::UpdateLeftPositionToMouse(*left_robot_, plates_[2].GetMouses()[0], clock_.GetElapsedTime())) {
				plates_[2].GetMouses().erase(plates_[2].GetMouses().begin());
				state_ = State::LEFTTOWORKPLACE;
			}
		}
		break;
	}
	case State::LEFTTOWORKPLACE: {
		if (Robot::UpdateLeftPositionMouseToWorkPlace(*left_robot_, Vec3{0.0f, -1.5f, 2.5f}, clock_.GetElapsedTime())) {
			state_ = State::LEFTGETBATTERY;
		}
		break;
	}
	case State::LEFTGETBATTERY: {
		Robot::left_mouse_.Draw();
		if (!battries_.empty() && Robot::UpdateLeftPositionToBattery(*left_robot_, Robot::GetClosestBattery(*left_robot_, battries_), clock_.GetElapsedTime())) {
			state_ = State::LEFTBATTERYTOWORKPLACE;
		}
		break;
	}
	case State::LEFTBATTERYTOWORKPLACE: {
		Robot::left_mouse_.Draw();
		if (Robot::UpdateLeftPositionBatteryToWorkPlace(*left_robot_, Vec3{ 0.0f, -1.5f, 2.5f }, clock_.GetElapsedTime())) {
			state_ = State::RIGHTGETBATTERY;
		}
		break;
	}
	case State::RIGHTGETBATTERY: {
		Robot::left_mouse_.Draw();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.2f, -1.8f, -3.5f);
		Robot::left_battery_.Draw();
		glPopMatrix();
		if (Robot::UpdateRightPositionToBattery(*right_robot_, Robot::GetClosestBattery(*right_robot_, battries_), clock_.GetElapsedTime())) {
			state_ = State::RIGHTBATTERYTOWORKPLACE;
		}
		break;
	}
	case State::RIGHTBATTERYTOWORKPLACE: {
		Robot::left_mouse_.Draw();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.2f, -1.8f, -3.5f);
		Robot::left_battery_.Draw();
		glPopMatrix();
		if (Robot::UpdateRightPositionBatteryToWorkPlace(*right_robot_, Vec3{ 0.0f, -1.5f, 2.5f }, clock_.GetElapsedTime())) {
			state_ = State::RIGHTGETTOMOUSEBASE;
		}
		break;
	}
	case State::RIGHTGETTOMOUSEBASE: {
		Robot::left_mouse_.Draw();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.2f, -1.8f, -3.5f);
		Robot::left_battery_.Draw();
		glPopMatrix();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.7f, -1.8f, -3.5f);
		Robot::right_battery_.Draw();
		glPopMatrix();
		if (!plates_[0].GetMouses().empty()) {
			if (Robot::UpdateRightPositionToMouse(*right_robot_, plates_[0].GetMouses()[0], clock_.GetElapsedTime())) {
				plates_[0].GetMouses().erase(plates_[0].GetMouses().begin());
				state_ = State::RIGHTTOWORKPLACE;
			}
		}
		break;
	}
	case State::RIGHTTOWORKPLACE: {
		Robot::left_mouse_.Draw();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.2f, -1.8f, -3.5f);
		Robot::left_battery_.Draw();
		glPopMatrix();
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.7f, -1.8f, -3.5f);
		Robot::right_battery_.Draw();
		glPopMatrix();
		if (Robot::UpdateRightPositionMouseToWorkPlace(*right_robot_, Vec3{ 0.0f, -1.5f, 2.5f }, clock_.GetElapsedTime())) {
			state_ = State::ASSEMBLE;
		}
		break;
	}
	case State::ASSEMBLE: {
		if (Robot::UpdateRightPositionToFinal(*right_robot_, Vec3{ 2.0f, -3.0f, 4.0f }, clock_.GetElapsedTime())) {
			plates_[1].GetMouses().push_back(Robot::mouse_);
			state_ = State::REINITIAL;
			plates_[2].GetMouses().push_back({ {-9.0f, -2.5f, 4.0f}, BASE });
			plates_[0].GetMouses().push_back({ {8.5f, -2.5f, -2.5f}, HEAD });
			left_base_rotate_ = static_cast<RobotBase*>(left_robot_.get())->GetRotate();
			left_arm2_rotate_ = static_cast<RobotArm*>(left_robot_->GetNext()->GetNext())->GetRotate() + 123.0;

			right_base_rotate_ = static_cast<RobotBase*>(right_robot_.get())->GetRotate();
			right_arm2_rotate_ = static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->GetRotate() - 123.0;
			timer_ = 1.0;
		}
		break;
	}
	case State::REINITIAL: {
		UpdateMouseHead(std::min(timer_, clock_.GetElapsedTime()));
		timer_ -= clock_.GetElapsedTime();
		if (timer_ <= 0) {
			state_ = State::LEFTGETTOMOUSEHEAD;
		}
	}
	}
}

void Animation::UpdateMouseHead(double time) {
	for (auto& mouse : plates_[0].GetMouses()) {
		mouse.SetPosition(mouse.GetPosition() + Vec3( -1.5 * time, 0.0f, 0.0f ));
	}

	for (auto& mouse : plates_[2].GetMouses()) {
		mouse.SetPosition(mouse.GetPosition() + Vec3( 1.5 * time, 0.0f, 0.0f ));
	}

	for (auto& mouse : plates_[1].GetMouses()) {
		mouse.SetPosition(mouse.GetPosition() + Vec3( 1.5 * time, 0.0f, 0.0f ));
	}

	static_cast<RobotBase*>(left_robot_.get())->SetRotate(static_cast<RobotBase*>(left_robot_.get())->GetRotate() - left_base_rotate_ * time);
	static_cast<RobotArm*>(left_robot_->GetNext()->GetNext())->SetRotate(static_cast<RobotArm*>(left_robot_->GetNext()->GetNext())->GetRotate() - left_arm2_rotate_ * time);

	static_cast<RobotBase*>(right_robot_.get())->SetRotate(static_cast<RobotBase*>(right_robot_.get())->GetRotate() - right_base_rotate_ * time);
	static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->SetRotate(static_cast<RobotArm*>(right_robot_->GetNext()->GetNext())->GetRotate() - right_arm2_rotate_ * time);

	Robot::left_mouse_.SetPosition( {0.0f, -0.2f, -1.2f} );
	Robot::right_mouse_.SetPosition( {0.0f, 0.0f, -1.2f} );
	Robot::left_battery_.SetPosition( {0.0f, 0.0f, -1.0f} );
	Robot::right_battery_.SetPosition( {0.0f, 0.0f, -1.0f} );
	Robot::mouse_.SetPosition( {0.0f, 0.0f, -1.2f} );
}