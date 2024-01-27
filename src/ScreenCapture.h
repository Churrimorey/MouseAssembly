#pragma once
#ifndef SCREEN_CAPTURE_H
#define SCREEN_CAPTURE_H

#include <string>
#include <GL/glut.h>
#include "utils.h" // 包括对应的工具头文件

class ScreenCapture {
public:
    static void captureScreenshot(const std::string& filename);
};

#endif // SCREEN_CAPTURE_H
