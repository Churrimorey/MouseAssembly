#include "drawings.h"
#include "material.h"

void Texture_desk()
{	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	Material::SetColor(1.0f, 1.0f, 1.0f);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();
	// glutSolidCube(2.0);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2i(1, 1); glVertex3i(-1, 1, 1);
	glTexCoord2i(1, 0); glVertex3i(-1, -1, 1);
	glTexCoord2i(0, 0); glVertex3i(1, -1, 1);
	glTexCoord2i(0, 1); glVertex3i(1, 1, 1);

	glNormal3f(0, 0, -1);
	glTexCoord2i(1, 1); glVertex3i(-1, 1, -1);
	glTexCoord2i(1, 0); glVertex3i(-1, -1, -1);
	glTexCoord2i(0, 0); glVertex3i(1, -1, -1);
	glTexCoord2i(0, 1); glVertex3i(1, 1, -1);

	glNormal3f(1, 0, 0);
	glTexCoord2i(1, 1); glVertex3i(1, -1, 1);
	glTexCoord2i(1, 0); glVertex3i(1, -1, -1);
	glTexCoord2i(0, 0); glVertex3i(1, 1, -1);
	glTexCoord2i(0, 1); glVertex3i(1, 1, 1);

	glNormal3f(-1, 0, 0);
	glTexCoord2i(1, 1); glVertex3i(-1, -1, 1);
	glTexCoord2i(1, 0); glVertex3i(-1, -1, -1);
	glTexCoord2i(0, 0); glVertex3i(-1, 1, -1);
	glTexCoord2i(0, 1); glVertex3i(-1, 1, 1);

	glNormal3f(0, 1, 0);
	for (int i=0; i < 64; i++) {
		for (int j=0; j < 64; j++) {
			glTexCoord2f(i / 64.0f, j / 64.0f);
			glVertex3f(-1 + 2 * i / 64.0f, 1, -1 + 2 * j / 64.0f);
			glTexCoord2f((i + 1) / 64.0f, j / 64.0f);
			glVertex3f(-1 + 2 * (i + 1) / 64.0f, 1, -1 + 2 * j / 64.0f);
			glTexCoord2f((i + 1) / 64.0f, (j + 1) / 64.0f);
			glVertex3f(-1 + 2 * (i + 1) / 64.0f, 1, -1 + 2 * (j + 1) / 64.0f);
			glTexCoord2f(i / 64.0f, (j + 1) / 64.0f);
			glVertex3f(-1 + 2 * i / 64.0f, 1, -1 + 2 * (j + 1) / 64.0f);
		}
	}
	// glDisable(GL_TEXTURE_2D);
	// glVertex3i(-1, 1, 1);
	// glVertex3i(-1, 1, -1);
	// glVertex3i(1, 1, -1);
	// glVertex3i(1, 1, 1);
	// glEnable(GL_TEXTURE_2D);

	glNormal3f(0, -1, 0);
	glTexCoord2i(1, 1); glVertex3i(-1, -1, 1);
	glTexCoord2i(1, 0); glVertex3i(-1, -1, -1);
	glTexCoord2i(0, 0); glVertex3i(1, -1, -1);
	glTexCoord2i(0, 1); glVertex3i(1, -1, 1);

	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void DrawTable()
{
	glPushMatrix();
	glTranslatef(0, -3.5, 0);
	glScalef(10, 0.5, 6);
	//glutSolidCube(1.0);
	Texture_desk();
	glPopMatrix();
}

void DrawCircle(float cx, float cy, float cz, float r, int num_segments)
{
	GLfloat vertex[4];

	const GLfloat delta_angle = 2.0 * PI / num_segments;
	glBegin(GL_TRIANGLE_FAN);

	vertex[0] = cx;
	vertex[1] = cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);

	//draw the vertex on the contour of the circle   
	for (int i = 0; i < num_segments; i++)
	{
		vertex[0] = std::cos(delta_angle * i) * r + cx;
		vertex[1] = std::sin(delta_angle * i) * r + cy;
		vertex[2] = cz;
		vertex[3] = 1.0;
		glVertex4fv(vertex);
	}

	vertex[0] = 1.0 * r + cx;
	vertex[1] = 0.0 * r + cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);
	glEnd();
}

void DrawCuboid(double cx, double cy, double cz, double halfX, double frontY, double backY, double halfZ)
{
	glBegin(GL_QUADS);
	// 左面
	glVertex3f(cx - halfX, cy, cz - halfZ);
	glVertex3f(cx - halfX, cy + backY, cz - halfZ);
	glVertex3f(cx - halfX, cy + frontY, cz + halfZ);
	glVertex3f(cx - halfX, cy, cz + halfZ);
	// 右面
	glVertex3f(cx + halfX, cy, cz - halfZ);
	glVertex3f(cx + halfX, cy + backY, cz - halfZ);
	glVertex3f(cx + halfX, cy + frontY, cz + halfZ);
	glVertex3f(cx + halfX, cy, cz + halfZ);
	// 上面
	glVertex3f(cx - halfX, cy + backY, cz - halfZ);
	glVertex3f(cx + halfX, cy + backY, cz - halfZ);
	glVertex3f(cx + halfX, cy + frontY, cz + halfZ);
	glVertex3f(cx - halfX, cy + frontY, cz + halfZ);
	// 下面
	glVertex3f(cx - halfX, cy, cz - halfZ);
	glVertex3f(cx + halfX, cy, cz - halfZ);
	glVertex3f(cx + halfX, cy, cz + halfZ);
	glVertex3f(cx - halfX, cy, cz + halfZ);
	// 前面
	glVertex3f(cx - halfX, cy, cz + halfZ);
	glVertex3f(cx + halfX, cy, cz + halfZ);
	glVertex3f(cx + halfX, cy + frontY, cz + halfZ);
	glVertex3f(cx - halfX, cy + frontY, cz + halfZ);
	// 后面
	glVertex3f(cx - halfX, cy, cz - halfZ);
	glVertex3f(cx + halfX, cy, cz - halfZ);
	glVertex3f(cx + halfX, cy + backY, cz - halfZ);
	glVertex3f(cx - halfX, cy + backY, cz - halfZ);
	glEnd();
}

void DrawDesk()
{
	glPushMatrix();
	glTranslatef(0.0f, -3.2f, 3.5f);
	DrawCuboid(0.5f, 0.0f, -0.8f, 1.0f, 2.0f, 2.0f, 1.0f);
	glPopMatrix();
}

void DrawHole()
{
	glPushMatrix();
	GLUquadricObj* cylinder = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glScalef(0.6f, 0.6f, 0.6f);
	gluCylinder(cylinder, 0.3, 0.3, 0.5, 10, 10);
	DrawCircle(0.0f, 0.0f, 0.5f, 0.3f, 10);
	gluDeleteQuadric(cylinder);
	glPopMatrix();
}

void DrawHoles()
{
	glPushMatrix();
	glTranslatef(-1.2f, 0.0f, -1.2f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, -1.2f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -1.2f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6f, 0.0f, -1.2f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2f, 0.0f, -1.2f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.2f, 0.0f, -0.6f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, -0.6f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.6f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6f, 0.0f, -0.6f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2f, 0.0f, -0.6f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.2f, 0.0f, 0.0f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, 0.0f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6f, 0.0f, 0.0f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2f, 0.0f, 0.0f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.2f, 0.0f, 0.6f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, 0.6f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.6f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6f, 0.0f, 0.6f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2f, 0.0f, 0.6f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.2f, 0.0f, 1.2f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, 1.2f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.2f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6f, 0.0f, 1.2f);
	DrawHole();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2f, 0.0f, 1.2f);
	DrawHole();
	glPopMatrix();
}

const GLfloat Battery::cone_mat[] =
{ 0.f, .5f, 1.f, 1.f };

GLUquadricObj* Battery::cylinder = gluNewQuadric();

const float Battery::positions[][3] = { {-1.2f, 0.0f, -1.2f}, {-0.6f, 0.0f, -1.2f}, {0.0f, 0.0f, -1.2f},
	{0.6f, 0.0f, -1.2f}, {1.2f, 0.0f, -1.2f}, {-1.2f, 0.0f, -0.6f}, {-0.6f, 0.0f, -0.6f}, {0.0f, 0.0f, -0.6f},
	{0.6f, 0.0f, -0.6f}, {1.2f, 0.0f, -0.6f}, {-1.2f, 0.0f, 0.0f}, {-0.6f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
	{0.6f, 0.0f, 0.0f}, {1.2f, 0.0f, 0.0f}, {-1.2f, 0.0f, 0.6f}, {-0.6f, 0.0f, 0.6f}, {0.0f, 0.0f, 0.6f},
	{0.6f, 0.0f, 0.6f}, {1.2f, 0.0f, 0.6f}, {-1.2f, 0.0f, 1.2f}, {-0.6f, 0.0f, 1.2f}, {0.0f, 0.0f, 1.2f},
	{0.6f, 0.0f, 1.2f}, {1.2f, 0.0f, 1.2f} };

void Battery::Draw() const {
	Material::SetMaterial(Material::Battery);
	glPushMatrix();
	glTranslatef(position_.GetX(), position_.GetY(), position_.GetZ());
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glScalef(0.6f, 0.6f, 0.6f);
	gluCylinder(cylinder, 0.3, 0.3, 1.0, 10, 10);
	DrawCircle(0.0f, 0.0f, 1.0f, 0.3f, 10);
	glPopMatrix();
}

void Battery::DrawBatterys(std::vector<Battery>& batteries) {
	glPushMatrix();
	glTranslatef(0.0f, -3.0f, 0.0f);

	for (auto& battery : batteries) {
		battery.Draw();
	}

	glPopMatrix();
}

void Battery::FillBatterys(std::vector<Battery>& batteries) {
	for (auto& position : positions) {
		batteries.emplace_back(Vec3{ position[0], position[1], position[2] });
	}
}

void DrawBox()
{
	glPushMatrix();
	glTranslatef(0.0f, -3.0f, 0.0f);
	//DrawHoles();
	DrawHoleList();
	glScalef(3.0f, 0.5f, 3.0f);
	glutSolidCube(1);
	glPopMatrix();
}

void GenHoleList()
{
	holeList[0] = glGenLists(25);
	for (int i = 1; i < 25; i++)
	{
		holeList[i] = holeList[0] + i;
	}

	glNewList(holeList[0], GL_COMPILE);
	glPushMatrix();
	glTranslatef(-1.2f, 0.0f, -1.2f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[1], GL_COMPILE);
	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, -1.2f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[2], GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -1.2f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[3], GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.6f, 0.0f, -1.2f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[4], GL_COMPILE);
	glPushMatrix();
	glTranslatef(1.2f, 0.0f, -1.2f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[5], GL_COMPILE);
	glPushMatrix();
	glTranslatef(-1.2f, 0.0f, -0.6f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[6], GL_COMPILE);
	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, -0.6f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[7], GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.6f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[8], GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.6f, 0.0f, -0.6f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[9], GL_COMPILE);
	glPushMatrix();
	glTranslatef(1.2f, 0.0f, -0.6f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[10], GL_COMPILE);
	glPushMatrix();
	glTranslatef(-1.2f, 0.0f, 0.0f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[11], GL_COMPILE);
	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, 0.0f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[12], GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[13], GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.6f, 0.0f, 0.0f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[14], GL_COMPILE);
	glPushMatrix();
	glTranslatef(1.2f, 0.0f, 0.0f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[15], GL_COMPILE);
	glPushMatrix();
	glTranslatef(-1.2f, 0.0f, 0.6f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[16], GL_COMPILE);
	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, 0.6f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[17], GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.6f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[18], GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.6f, 0.0f, 0.6f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[19], GL_COMPILE);
	glPushMatrix();
	glTranslatef(1.2f, 0.0f, 0.6f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[20], GL_COMPILE);
	glPushMatrix();
	glTranslatef(-1.2f, 0.0f, 1.2f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[21], GL_COMPILE);
	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, 1.2f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[22], GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.2f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[23], GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.6f, 0.0f, 1.2f);
	DrawHole();
	glPopMatrix();
	glEndList();

	glNewList(holeList[24], GL_COMPILE);
	glPushMatrix();
	glTranslatef(1.2f, 0.0f, 1.2f);
	DrawHole();
	glPopMatrix();
	glEndList();
}

void DrawHoleList()
{
	for (int i = 0; i < 25; i++)
	{
		glCallList(holeList[i]);
	}
}