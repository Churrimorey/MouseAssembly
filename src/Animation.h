#ifndef __ANIMATION_H_
#define __ANIMATION_H_

#include <memory>
#include "clock.h"
#include "robot.h"

extern bool bAnim;

/**
 * Animation - 统管动画
 */
class Animation {
public:
	Animation()
		: left_robot_(Robot::CreateLeftRobot()), right_robot_(Robot::CreateRightRobot()),
		  clock_(false), state_(State::LEFTGETTOMOUSEHEAD) {}

	enum class State { LEFTGETTOMOUSEHEAD, LEFTTOWORKPLACE,
					   LEFTGETBATTERY, LEFTBATTERYTOWORKPLACE,
					   RIGHTGETBATTERY, RIGHTBATTERYTOWORKPLACE,
					   RIGHTGETTOMOUSEBASE, RIGHTTOWORKPLACE,
					   ASSEMBLE, REINITIAL };

	void SetUpdate(bool update) { clock_.SetUpdate(update); }

	void Update();

	const Robot* GetLeftRobot() const { return left_robot_.get(); }

	const Robot* GetRightRobot() const { return right_robot_.get(); }

	std::vector<Battery>& GetBattery() { return battries_; }

	std::vector<MousePlate>& GetMousePlates() { return plates_; }

private:
	void UpdateLeftHead(double time);
	void UpdateRightHead(double time);

	std::unique_ptr<Robot> left_robot_;
	std::unique_ptr<Robot> right_robot_;
	std::vector<MousePlate> plates_;
	std::vector<Battery> battries_;
	Clock clock_;
	State state_;
	double timer_{1.0};
	float left_base_rotate_{36.0f};
	float left_arm1_rotate_{0.0f};
	float left_arm2_rotate_{0.0f};
	float right_base_rotate_;
	float right_arm1_rotate_;
	float right_arm2_rotate_;
};

#endif
