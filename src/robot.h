#ifndef __ROBOT_H_
#define __ROBOT_H_

#include <gl/glut.h>
#include "drawings.h"
#include "Vec3.h"

class Robot {
public:
    explicit Robot(const Vec3& position, Robot* next = nullptr) : position_(position), next_(next) {}

    virtual ~Robot() { delete next_; }

    static Robot* CreateLeftRobot();

    static Robot* CreateRightRobot();

    const Vec3& GetPosition() const { return position_; }

    void SetNext(Robot* next) { next_ = next; }

    Robot* GetNext() { return next_; }
    
    const Robot* GetNext() const { return next_;  }

    virtual void Draw() const = 0;

    void DrawRobot() const;

private:
    Vec3 position_;
    Robot* next_;
};

class RobotBase : public Robot {
public:
    explicit RobotBase(const Vec3& position, Robot* next = nullptr) : Robot(position, next) {}

    void Draw() const override;
};

class RobotArm : public Robot {
public:
    explicit RobotArm(const Vec3& position, float rotate, Robot* next = nullptr) : Robot(position, next), rotate_(rotate) {}

    void Draw() const override;

    void SetRotate(float rotate) { rotate_ = rotate; }

private:
    float rotate_;
};

class RobotFinger : public Robot {
public:
    explicit RobotFinger(const Vec3& position, Robot* next = nullptr) : Robot(position, next) {}

    void Draw() const override;
};

#endif