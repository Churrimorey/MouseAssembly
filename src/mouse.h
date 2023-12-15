#ifndef __MOUSE_H_
#define __MOUSE_H_

#include <gl/glut.h>
#include <vector>
#include "Vec3.h"

class Mouse {
public:
	enum class Type { HEAD = 1, BASE };

	Mouse(const Vec3 & position, Type type) : position_(position), type_(type) {}

	void Draw() const;

	static void DrawMouse(int num);

private:

	static short face_indicies[][3];
	Vec3 position_;
	Type type_;
};

class MousePlate {
public:
	static void DrawPlates(std::vector<MousePlate> &plates);

	MousePlate(const Vec3 &position) : position_(position) {}

	void DrawMousePlates() const;

	std::vector<Mouse>& GetMouses() { return mouse_;  }

	void DrawMouseHeads();

	void DrawMouseBases();

private:
	std::vector<Mouse> mouse_;
	Vec3 position_;
};

#endif