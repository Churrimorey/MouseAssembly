#ifndef __UTILS_H_
#define __UTILS_H_

#include <cstdio>
#include <gl/glut.h>
#define BITMAP_ID 0x4D42

extern unsigned int texture[64];

#ifdef MACOS
#include <sys/types.h>
#include <sys/stat.h>

#pragma pack(2)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

#pragma pack(2)
typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#else
#include <Windows.h>
#endif
bool SaveBitmapFile(const char* filename, const unsigned char* data, int width, int height);
unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
void texload(int i, char* filename);
#endif
