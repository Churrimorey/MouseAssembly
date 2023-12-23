#include "light.h"
#include "vec_utils.h"

using namespace set_fv;

GLfloat Light::light_position_[light_num_max][4];
enum Light::LightType Light::light_type_[light_num_max];
GLfloat Light::light_direction_[light_num_max][3];

GLfloat Light::light_ambient_[LightType::Count][4];
GLfloat Light::light_diffuse_[LightType::Count][4];
GLfloat Light::light_specular_[LightType::Count][4];

void Light::InitLight() {
    glEnable(GL_LIGHTING);

    for (int i = 0; i < light_num_max; i++) {
        light_position_[i][0] = 0.0f;
        light_position_[i][1] = 5.0f;
        light_position_[i][2] = 0.0f;
        light_position_[i][3] = 0.0f;
        light_type_[i] = LightType::DirectionalLight;
        light_direction_[i][0] = 0.0f;
        light_direction_[i][1] = 0.0f;
        light_direction_[i][2] = 0.0f;
        
    }

    // 设置不同类型灯光的效果
    // DirectionalLight
    set_4fv(light_ambient_[DirectionalLight], 0xa0a0a0U);
    set_4fv(light_diffuse_[DirectionalLight], 0x202020);
    set_4fv(light_specular_[DirectionalLight], 0x303030U);

    // PointLight
    set_4fv(light_ambient_[PointLight], 0x101010U);
    set_4fv(light_diffuse_[PointLight], 0x080808U);
    set_4fv(light_specular_[PointLight], 0x080808U);
}

void Light::TurnOnLight(int light_id) {
    glLightfv(GL_LIGHT0 + light_id, GL_POSITION, light_position_[light_id]);
    LightType type = light_type_[light_id];
    glLightfv(GL_LIGHT0 + light_id, GL_AMBIENT, light_ambient_[type]);
    glLightfv(GL_LIGHT0 + light_id, GL_DIFFUSE, light_diffuse_[type]);
    glLightfv(GL_LIGHT0 + light_id, GL_SPECULAR, light_specular_[type]);
    glEnable(GL_LIGHT0 + light_id);
}

void Light::SetLightPosition(int light_id, float x, float y, float z, float w) {
    light_position_[light_id][0] = x;
    light_position_[light_id][1] = y;
    light_position_[light_id][2] = z;
    light_position_[light_id][3] = w;
}

void Light::SetLightType(int light_id, enum LightType type) {
    light_type_[light_id] = type;
}

void Light::TurnOffLight(int light_id) {
    glDisable(GL_LIGHT0 + light_id);
}