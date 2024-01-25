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

    // ����һ���ڴ�ռ����ڴ洢��ͼ
    auto* buffer = new GLubyte[width * height * 3];
    if (!buffer) {
        // �ڴ����ʧ�ܵĴ�����
        std::cerr << "Error: Not enough memory to save screenshot." << std::endl;
        return;
    }

    // ��OpenGL��ȡ����
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    // ʵ�ʵı����߼�
    if (!SaveBitmapFile(filename.c_str(), buffer, width, height)) {
        // �������ʧ�ܣ���ӡ������Ϣ
        std::cerr << "Error: Screenshot saving failed." << std::endl;
    }

    // �ͷ��ڴ�
    delete[] buffer;
}