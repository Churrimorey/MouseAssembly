#ifndef __UTILS_H_
#define __UTILS_H_

#include <gl/glut.h>
#include <Windows.h>
#include <cstdio>

#define BITMAP_ID 0x4D42

extern unsigned int texture[2];

unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
void texload(int i, char* filename);

#endif
