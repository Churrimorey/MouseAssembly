#pragma once
#include <gl/glut.h>

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
    static GLfloat light_direction_[light_num_max][3];

    // 设置不同类型灯光的效果
    static GLfloat light_ambient_[LightType::Count][4];
    static GLfloat light_diffuse_[LightType::Count][4];
    static GLfloat light_specular_[LightType::Count][4];

public:
    static void InitLight();
    static void TurnOnLight(int light_id);
    static void SetLightPosition(int light_id, float x, float y, float z, float w);
    static void SetLightType(int light_id, enum LightType type);
    static void TurnOffLight(int light_id);
};
