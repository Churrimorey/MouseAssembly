#ifndef __VEC3_H_
#define __VEC3_H_

#include <gl/glut.h>

class Vec3 {
public:
    Vec3() = default;

    Vec3(float x, float y, float z) noexcept : x_(x), y_(y), z_(z) {}

    Vec3(const Vec3& rhs) = default;

    Vec3& operator=(const Vec3& rhs) = default;

    Vec3 Unit() const;

    float Length() const;

    void SetX(float x) { x_ = x; }

    float GetX() const { return x_; }

    void SetY(float y) { y_ = y; }

    float GetY() const { return y_; }

    void SetZ(float z) { z_ = z; }

    float GetZ() const { return z_; }

    float Cross(const Vec3& rhs) const;

    float CrossXZ(const Vec3& rhs) const;

    float LengthXZ() const;

    Vec3 operator*(const GLfloat* matrix) const;

    Vec3 operator*(float f) const;

    Vec3 operator/(float f) const;

    Vec3& operator+=(const Vec3& rhs);

private:
    float x_;
    float y_;
    float z_;
};

Vec3 operator-(const Vec3& left, const Vec3& right);

Vec3 operator+(const Vec3& left, const Vec3& right);


#endif