#pragma once
#ifndef SCREEN_CAPTURE_H
#define SCREEN_CAPTURE_H

#include <string>
#include <GL/glut.h>
#include "utils.h" // ������Ӧ�Ĺ���ͷ�ļ�

class ScreenCapture {
public:
    static void captureScreenshot(const std::string& filename);
};

#endif // SCREEN_CAPTURE_H
