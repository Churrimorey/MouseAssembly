#include "ScreenCapture.h"
#include "utils.h" 
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>

void ScreenCapture::captureScreenshot(const std::string& filename) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int width = viewport[2];
    int height = viewport[3];

    // 开辟一段内存空间用于存储截图
    auto* buffer = new GLubyte[width * height * 3];
    if (!buffer) {
        // 内存分配失败的错误处理
        std::cerr << "Error: Not enough memory to save screenshot." << std::endl;
        return;
    }

    // 从OpenGL读取像素
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    // 实际的保存逻辑
    if (!SaveBitmapFile(filename.c_str(), buffer, width, height)) {
        // 如果保存失败，打印错误信息
        std::cerr << "Error: Screenshot saving failed." << std::endl;
    }

    // 释放内存
    delete[] buffer;
}