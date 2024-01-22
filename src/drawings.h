#ifndef __DRAWINGS_H_
#define __DRAWINGS_H_

#include <gl/glut.h>
#include <cmath>
#include <vector>
#include "Vec3.h"

#define PI 3.1415926535

extern unsigned int texture[9];
extern GLint holeList[25];
extern GLint batterList[25];

void Texture_desk();
void DrawTable();
void DrawCircle(float cx, float cy, float cz, float r, int num_segments);
void DrawCuboid(double cx, double cy, double cz, double halfX, double frontY, double backY, double halfZ);
void DrawDesk();
void DrawHole();
void DrawHoles();
void DrawBox();
void GenHoleList();
void DrawHoleList();

class Battery {
public:
	static const GLfloat cone_mat[];

	static GLUquadricObj* cylinder;

	static const float positions[][3];

	static void DrawBatterys(std::vector<Battery>& batteries);

	static void FillBatterys(std::vector<Battery>& batteries);

	static void DrawBattery(float x, float y, float z) {
		Battery battery(Vec3{ x, y, z });
		battery.Draw();
	}

	Battery() = default;

	Battery(const Vec3 &position) : position_(position) {}

	const Vec3& GetPosition() const { return position_; }

	void SetPosition(const Vec3& position) { position_ = position; }

	void Draw() const;

private:
	Vec3 position_;
};

#endif
