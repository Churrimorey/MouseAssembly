#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <iostream>
#include <memory>
#include <vector>
#include "Animation.h"
#include "mouse.h"
#include "utils.h"
#include "drawings.h"
#include "robot.h"
#include "light.h"
#include "material.h"
#include "clock.h"
#include "menu.h"
#include "getFPS.h"
#include <math.h>
#include "ScreenCapture.h"
#pragma comment (lib,"glew32.lib")
#define PI 3.1415926535
#define STEP 1 // 视角平移的系数
#define GLUT_KEY_SHIFT_L 97
#define GLUT_KEY_SHIFT_R 98
#define PLAY 1
int gHeight;
int gWidth;

float eye[] = { 0, 4, 8 }; // camera coordinates
float center[] = { 0, 0, 0 }; //center coordinates
static int du = 90; //du是视点和x轴的夹角
static int dv = 0;
static int OriX = -1, OriY = -1;  

static float r = 10, h = 0.0;   //r是视点绕y轴的半径，h是视点高度即在y轴上的坐标
static float c = PI / 180.0;    //弧度和角度转换参数
double step = 0;
double height = 0;

unsigned int texture[9];
bool bMouseBase = false;
bool bMouseHead = false;
bool bBattery = false;
bool bAnim = false;
GLint holeList[25];
GLint batterList[25];
Animation animation;
Menu menu(2);

Mouse head = Mouse(Vec3(0, 0, 0), HEAD);
Mouse base = Mouse(Vec3(0, 0, 0), BASE);

GLUnurbsObj *theNurb;

void processMenuEvents(int option) {
	switch (option) {
	case PLAY:
		bAnim = true;
		break;
	default:
		break;
}

	//glutPostRedisplay();
}


void createGLUTMenus() {

	int menu;

	menu = glutCreateMenu(processMenuEvents);

	glutAddMenuEntry("Run", PLAY);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init()
{
	GLint buf[1], sbuf[1];
	glGetIntegerv(GL_SAMPLE_BUFFERS_ARB, buf);
	glGetIntegerv(GL_SAMPLES_ARB, sbuf);
	glEnable(GL_MULTISAMPLE_ARB);

	glGenTextures(9, texture);
	texload(0, (char*)"table.bmp");
	texload(1, (char*)"battery.bmp");
	texload(2, (char*)"ar.bmp");
	texload(3, (char*)"nx.bmp");
	texload(4, (char*)"ny.bmp");
	texload(5, (char*)"nz.bmp");
	texload(6, (char*)"px.bmp");
	texload(7, (char*)"py.bmp");
	texload(8, (char*)"pz.bmp");

	MousePlate::FillPlates(animation.GetMousePlates());

	Light::InitLight(menu);
	Material::InitMaterial();
	ARabout::InitAR(menu);

	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
}

// 获取并打印模型视图矩阵
void printModelViewMatrix() {
	GLfloat modelViewMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
	std::cout << "Model View Matrix:" << std::endl;
	for (int i = 0; i < 16; i++) {
		std::cout << modelViewMatrix[i] << " ";
		if ((i + 1) % 4 == 0) std::cout << std::endl;
	}
}

void DrawScene()
{
	DrawTable();

	animation.GetLeftRobot()->DrawRobot();

	animation.GetRightRobot()->DrawRobot();

	Material::SetMaterial(Material::Desk);
	DrawDesk();

	Material::SetMaterial(Material::Box);
	DrawBox();

	if (bBattery)
	{
		Battery::FillBatterys(animation.GetBattery());
		bBattery = false;
	}
	Battery::DrawBatterys(animation.GetBattery());
	Material::SetMaterial(Material::Plate);
	MousePlate::DrawPlates(animation.GetMousePlates());

	if (bMouseHead)
	{
		animation.GetMousePlates()[0].FillMouseHeads();
		bMouseHead = false;
	}
	animation.GetMousePlates()[0].DrawMouseHeads();

	if (bMouseBase)
	{
		animation.GetMousePlates()[2].FillMouseBases();
		bMouseBase = false;
	}
	animation.GetMousePlates()[2].DrawMouseBases();

	animation.GetMousePlates()[1].DrawMouses();

	//glPushMatrix();
	////printModelViewMatrix();
	////glLoadIdentity();
	//glTranslatef(2.0f, -3.0f, 5.5f);
	//glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	//glScalef(0.60f, 0.60f, 0.60f);
	//head.DrawMouse(HEAD);
	//base.DrawMouse(BASE);
	//glPopMatrix();
}

void DrawEditBar()
{
	glPushMatrix();
	glTranslatef(-3.3f, 2.9f, 0.0f);
	glScalef(0.2f, 0.2f, 0.2f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	head.DrawMouse(HEAD);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.3f, 1.7f, 0.0f);
	glScalef(0.2f, 0.2f, 0.2f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	base.DrawMouse(BASE);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.6f, -0.05f, 0.0f);
	glScalef(0.5f, 1.0f, 0.5f);
	Battery::DrawBattery(0, 0, 0);
	glPopMatrix();

	Material::SetMaterial(Material::UIGreen);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(-3.2f, 2.0f, 0.0f);
	glVertex3f(-4.0f, 2.0f, 0.1f);

	glVertex3f(-3.2f, 0.8f, 0.0f);
	glVertex3f(-4.0f, 0.8f, 0.1f);

	glVertex3f(-3.2f, -0.3f, 0.0f);
	glVertex3f(-4.0f, -0.3f, 0.1f);
	glEnd();
}

void reshape(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}

	gHeight = height;
	gWidth = width;

	/*glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float whRatio = (GLfloat)width / (GLfloat)height;
	gluPerspective(45, whRatio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);		*/
}

void idle()
{
	if (bAnim) // 如果需要播放动画
	{
		animation.Update(); // 更新动画的状态
	}
	glutPostRedisplay(); // 通知GLUT重绘屏幕
}
void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
	{
		height += STEP;
		break;
	}	
	case GLUT_KEY_DOWN:
	{
		height -= STEP;
		break;
	}
	default:
		break;
	}
}
void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; }

	case '+':
	{

		break;
	}
	case '-':
	{

		break;
	}
	// change perspective
	case 'w': {
		r -= STEP;
		break;
	}
	case 's': {
		r += STEP;
		break;
	}
	case 'd': {
		step += STEP;
		break;
	}
	case 'a': {
		step -= STEP;
		break;
	}
	case 'f': // 按 'F' 字母键截图
	{
		ScreenCapture::captureScreenshot("screenshot.bmp");   //可换成希望保存到的路径和文件名
		std::cout << "Screenshot saved to screenshot.bmp" << std::endl;
		break;
	}
	default: break;
	}
}
bool NoWalkTh = 0; // when the left button is not in down state, not walkthrough
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (ARabout::Hit(x, gHeight - y))
			return;
		if (menu.Hit(x, gHeight - y))
			return;
		printf("%d %d\n", x, y);
		if (x <= 110)
		{
			if (y >= 0 && y <= 86)
			{
				bMouseHead = true;
			}
			else if (y <= 175)
			{
				bMouseBase = true;
			}
			else if (y <= 252)
			{
				bBattery = true;
			}
		}
	}
	else {
		NoWalkTh = 0;
	}
}

void onMouseMove(int x, int y)   //处理鼠标拖动
{
	if (NoWalkTh == 0) { // if not walkthrough, just save the point
		NoWalkTh = 1;
		OriX = x;
		OriY = y;
	}
	du += x - OriX; //鼠标在窗口x轴方向上的增量加到视点与x轴的夹角上，就可以左右转
	dv += y - OriY;  //鼠标在窗口y轴方向上的改变加到视点y的坐标上，就可以上下转
	h += 0.03 * (y - OriY);  //鼠标在窗口y轴方向上的改变加到视点y的坐标上，就可以上下转
	if (h > 1.0)   h = 1.0;  //对视点y坐标作一些限制，不会使视点太奇怪
	else if (h < -1.0) h = -1.0;
	OriX = x, OriY = y;  //将此时的坐标作为旧值，为下一次计算增量做准备

}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);

	glFrontFace(GL_CW);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glLoadIdentity();
	glPushMatrix();
	glViewport(0, 0, gWidth, gHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float whRatio = (GLfloat)(gWidth - 80) / (GLfloat)gHeight;
	gluPerspective(45, whRatio, 1, 1000);
	if (du > 360) du -= 360;
	if (du < -360) du += 360;
	if (dv > 360) dv -= 360;
	if (dv < -360) dv += 360;
	//eye[0] = r * cos(c * du) * cos(c * dv);
	//eye[1] = r * sin(c * dv);
	//eye[2] = r * cos(c * dv) * sin(c * du);
	//printf_s("eye = %f,%f,%f\n", eye[0], eye[1], eye[2]);
	gluLookAt(r * cos(c * du) + step * sin(c * du), h + height, r * sin(c * du) - step * cos(c * du), step * sin(c * du), height, -step * cos(c * du), 0.0, 1.0, 0.0);   //从视点看远点
	//gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScalef(0.5, 0.5, 0.5);
	Light::FlushLight();
	Light::DrawSkyBox();
	DrawScene();
	if (bAnim)
	{
		if (!animation.GetMousePlates()[2].IsEmpty()) {
			animation.SetUpdate(true);
		}
		else
		{
			bAnim = false;
		}
	}
	else
	{
		animation.SetUpdate(false);
	}
	animation.Update();
	glPopMatrix();

	if(!bAnim)
	{
		// Draw Edit Bar
		Light::FlushEditBarLight();
		glPushMatrix();
		glViewport(0, 0, gWidth, gHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-3, 3, -3, 3, -100, 100);
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(eye[0], eye[1], eye[2],
			-1.0f, 0.0f, 0.0f,
			0, 1, 0);
		DrawEditBar();
		glPopMatrix();
	}

	// Draw 2D UI
	Material::SetMaterial(Material::UIGreen);
	glPushMatrix();
	glViewport(0, 0, gWidth, gHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, gWidth, 0, gHeight);
	glMatrixMode(GL_MODELVIEW);
	ARabout::Draw();
	menu.Draw(0, 0);
	getFPS();
	glPopMatrix();

	//draw view center point
	// glPushMatrix();
	// glViewport(0, 0, gWidth, gHeight);
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// gluOrtho2D(0, gWidth, 0, gHeight);
	// glMatrixMode(GL_MODELVIEW);
	// Material::SetMaterial(Material::Unknown);	
	// glPointSize(10.0f);
	// glBegin(GL_POINTS);
	// glVertex3f((gWidth - 80) / 2 + 80, gHeight / 2, 0.0f);
	// glEnd();
	// glPopMatrix();

	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 480);
	int windowHandle = glutCreateWindow("Mouse Assembly Workshop");
	GenHoleList();
	init();
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutSpecialFunc(specialKeys);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(onMouseMove);
	createGLUTMenus();

	glutMainLoop();
	return 0;
}
