#include "Vec3.h"
#include <cmath>

Vec3 operator-(const Vec3& left, const Vec3& right) {
	return Vec3{ left.GetX() - right.GetX(), left.GetY() - right.GetY(), left.GetZ() - right.GetZ() };
}

Vec3 operator+(const Vec3& left, const Vec3& right) {
	return Vec3{ left.GetX() + right.GetX(), left.GetY() + right.GetY(), left.GetZ() + right.GetZ() };
}

Vec3 Vec3::operator*(const GLfloat* matrix) const {
	float res[3] = { 0 };
	for (int i = 0; i < 3; i++) {
		res[i] = x_ * matrix[i * 4] + y_ * matrix[i * 4 + 1] + z_ * matrix[i * 4 + 2] + matrix[i * 4 + 3];
	}
	return { res[0], res[1], res[2] };
}

Vec3 Vec3::operator*(float f) const {
	return Vec3{ x_ * f, y_ * f, z_ * f };
}

Vec3 Vec3::operator/(float f) const {
	return Vec3{ x_ / f, y_ / f, z_ / f };
}

Vec3& Vec3::operator+=(const Vec3& rhs) {
	x_ += rhs.x_;
	y_ += rhs.y_;
	z_ += rhs.z_;
	return *this;
}

float Vec3::Length() const {
	return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

Vec3 Vec3::Unit() const {
	return *this / Length();
}

float Vec3::Cross(const Vec3& rhs) const {
	return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_;
}

float Vec3::CrossXZ(const Vec3& rhs) const {
	return x_ * rhs.x_ + z_ * rhs.z_;
}

float Vec3::LengthXZ() const {
	return sqrt(x_ * x_ + z_ * z_);
}