#include "vec_utils.h"

void set_fv::set_4fv(GLfloat* arr, GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
    arr[0] = a;
    arr[1] = b;
    arr[2] = c;
    arr[3] = d;
}

void set_fv::set_4fv(GLfloat* arr, uint32_t color) {
        arr[0] = 1.0f * ((color >> 16) & 0xff) / 255;
        arr[1] = 1.0f * ((color >> 8) & 0xff) / 255;
        arr[2] = 1.0f * (color & 0xff) / 255;
        arr[3] = 1.0f;
}

GLfloat* set_fv::coeff_4fv(GLfloat* arr, GLfloat coeff) {
    GLfloat* ret = new GLfloat[4];
    for (int i = 0; i < 3; i++) {
        ret[i] = arr[i] * coeff;
    }
    ret[3] = 1.0;
    return ret;
}