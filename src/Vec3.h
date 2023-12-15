#ifndef __VEC3_H_
#define __VEC3_H_

class Vec3 {
public:
    Vec3(float x, float y, float z) : x_(x), y_(y), z_(z) {}

    float GetX() const { return x_; }

    float GetY() const { return y_; }

    float GetZ() const { return z_; }

private:
    float x_;
    float y_;
    float z_;
};

#endif