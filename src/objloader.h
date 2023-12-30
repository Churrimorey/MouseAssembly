# include <GL/glut.h>

#pragma once
#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <regex>
using namespace std;

const struct OBJ_COLOR {
	GLfloat red, green, blue;
	OBJ_COLOR() : red(1.0), green(1.0), blue(1.0) {}
} OBJ_COLOR;

typedef struct vertex {
	double x, y, z;
} vertex;
typedef struct face_triangle {
	unsigned long v1, v2, v3;
} face_triangle;
typedef struct face_quad {
	unsigned long v1, v2, v3, v4;
} face_quad;



class ObjLoader
{
public:
	ObjLoader();
	bool render_mode; // true = solid body, false = wireframe
	void open_obj(const char* filename);
	void draw_obj() const ;
	std::vector<vertex> vertices;
	std::vector<face_quad> faces_quads;
	std::vector<face_triangle> faces_triangles;
private:
	bool is_quad;
	void obj_parse(char* file_line);
	void calculate_normal(face_triangle f, GLdouble* normal)const ;
};