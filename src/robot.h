#ifndef __ROBOT_H_
#define __ROBOT_H_

#include <gl/glut.h>
#include "drawings.h"
#include "Vec3.h"
#include "mouse.h"

class RobotArm;
class RobotBase;

class Robot {
public:
    static Mouse left_mouse_;

    static Mouse right_mouse_;

    static Battery left_battery_;

    static Battery right_battery_;

    explicit Robot(const Vec3& position, Robot* next = nullptr) : position_(position), next_(next) {}

    virtual ~Robot() { delete next_; }

    static Robot* CreateLeftRobot();

    static Robot* CreateRightRobot();

    const Vec3& GetPosition() const { return position_; }

    void SetPosition(const Vec3& position) { position_ = position; }

    void SetNext(Robot* next) { next_ = next; }

    Robot* GetNext() { return next_; }
    
    const Robot* GetNext() const { return next_;  }

    virtual void Draw() const = 0;

    void DrawRobot() const;

    static Battery& GetClosestBattery(Robot& robot, std::vector<Battery>& batteries);

    static bool UpdateLeftPositionToMouse(Robot &robot, const Mouse& destination, double elapsed_time);

    static bool UpdateLeftPositionToBattery(Robot& robot, const Battery& destination, double elapsed_time);

    static bool UpdateRightPositionToMouse(Robot& robot, const Mouse& destination, double elapsed_time);

    static bool UpdateRightPositionToBattery(Robot& robot, const Battery& destination, double elapsed_time);

    static void UpdateLeftPositionToMouse(RobotBase& base, RobotArm& arm1, RobotArm& arm2, const Vec3& position);

    static void UpdateRightPositionToMouse(RobotBase& base, RobotArm& arm1, RobotArm& arm2, const Vec3& position);

    static void UpdateLeftPositionToBattery(RobotBase& base, RobotArm& arm1, RobotArm& arm2, const Vec3& position);

    static void UpdateRightPositionToBattery(RobotBase& base, RobotArm& arm1, RobotArm& arm2, const Vec3& position);

private:
    Vec3 position_;
    Robot* next_;
};

class RobotBase : public Robot {
public:
    static GLUquadricObj* cone;

    RobotBase(const Vec3& position, float rotate, Robot* next = nullptr) : Robot(position, next), rotate_(rotate) {}

    void Draw() const override;

    float GetRotate() const { return rotate_; }

    void SetRotate(float rotate) { rotate_ = rotate; }

private:
    float rotate_;
};

class RobotArm : public Robot {
public:
    /*static GLfloat cone_mat[];

    static GLUquadricObj* cylinder;*/

    explicit RobotArm(const Vec3& position, float rotate, Robot* next = nullptr) : Robot(position, next), prev_(nullptr), rotate_(rotate) {}

    virtual void Draw() const = 0;

    void SetRotate(float rotate) { rotate_ = rotate; }

    float GetRotate() const { return rotate_;  }

    void SetPrev(Robot* prev) { prev_ = prev; }

    Vec3 GetLocalPosition() const;

protected:
    Robot* prev_;
    float rotate_;
};

class RobotRightArm : public RobotArm {
public:
	explicit RobotRightArm(const Vec3& position, float rotate, Robot* next = nullptr) : RobotArm(position,rotate,next){}
	void Draw() const override;
	static GLfloat ctrlpoints[5][8][3];
};

class RobotLeftArm : public RobotArm {
public:
	explicit RobotLeftArm(const Vec3& position, float rotate, Robot* next = nullptr) : RobotArm(position, rotate, next) {}
	void Draw() const override;
	static GLfloat ctrlpoints[5][8][3];
};

class RobotFinger : public Robot {
public:
    explicit RobotFinger(const Vec3& position, Robot* next = nullptr) : Robot(position, next) {}

    void Draw() const override;
};

#endif