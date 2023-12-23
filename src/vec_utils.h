#pragma once
#include <gl/glut.h>

namespace set_fv {
    void set_4fv(GLfloat* arr, GLfloat a, GLfloat b, GLfloat c, GLfloat d);
    void set_4fv(GLfloat* arr, uint32_t color);
}