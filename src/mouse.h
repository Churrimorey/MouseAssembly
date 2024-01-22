#ifndef __MOUSE_H_
#define __MOUSE_H_

#include <gl/glut.h>
#include <vector>
#include "Vec3.h"
#include "objloader.h"
enum  Type { HEAD = 1, BASE, WHOLE};

class Mouse {
public:
	Mouse() = default;

	Mouse(const Vec3& position, Type type);

	void Draw() const;

	const Vec3& GetPosition() const { return position_; }

	void SetPosition(const Vec3& position) { position_ = position;  }

	void SetType(Type type) { type_ = type; }

	void DrawMouse(Type type)const;
private:
	ObjLoader base;
	ObjLoader head;
	Vec3 position_;
	Type type_;
};

class MousePlate {
public:
	static void DrawPlates(std::vector<MousePlate> &plates);

	static void FillPlates(std::vector<MousePlate>& plates);

	MousePlate(const Vec3& position) : position_(position) {};

	void DrawMousePlates() const;

	std::vector<class Mouse>& GetMouses() { return mouse_;  }

	void DrawMouseHeads() const;

	void FillMouseHeads();

	void DrawMouseBases() const;

	void FillMouseBases();

	void DrawMouses() const;

	void UpdateEmpty() { empty_ = mouse_.empty(); }

	bool IsEmpty() { return empty_; }

private:
	std::vector<class Mouse> mouse_;
	Vec3 position_;
	static Vec3 mouseHeadPos[4];
	static Vec3 mouseBasePos[4];
	bool empty_ = true;
};

#endif