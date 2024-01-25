#pragma once
#include <gl/glut.h>
#include "menu.h"

class Light {
public:
    enum LightType {
        PointLight = 0,
        DirectionalLight,
        Count
    };

private:
    static const int light_num_max = 8;

    static GLfloat light_position_[light_num_max][4];
    static enum LightType light_type_[light_num_max];
    static GLfloat light_brightness_[light_num_max];
    static bool light_on_[light_num_max];

    // 设置不同类型灯光的效果
    static GLfloat light_ambient_[LightType::Count][4];
    static GLfloat light_diffuse_[LightType::Count][4];
    static GLfloat light_specular_[LightType::Count][4];

public:
    static void DrawSkyBox();
    static void InitLight(Menu& menu);
    static void FlushLight();
    static void FlushEditBarLight();
    static void TurnOnLight(int light_id);
    static void MoveLightPosition(int light_id, float x, float y, float z, float w);
    static void SetLightType(int light_id, enum LightType type);
    static void AdjLightBrightness(int light_id, GLfloat dx);
    static void TurnOffLight(int light_id);
};
