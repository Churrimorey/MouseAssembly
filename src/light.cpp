#include "light.h"
#include "vec_utils.h"

using namespace set_fv;

GLfloat Light::light_position_[light_num_max][4];
enum Light::LightType Light::light_type_[light_num_max];
GLfloat Light::light_brightness_[light_num_max];
bool Light::light_on_[light_num_max];

GLfloat Light::light_ambient_[LightType::Count][4];
GLfloat Light::light_diffuse_[LightType::Count][4];
GLfloat Light::light_specular_[LightType::Count][4];

void Light::InitLight(Menu& menu) {
    glEnable(GL_LIGHTING);

    light_position_[0][0] = 0.0f;
    light_position_[0][1] = 5.0f;
    light_position_[0][2] = 0.0f;
    light_position_[0][3] = 0.0f;
    light_type_[0] = LightType::DirectionalLight;
    light_brightness_[0] = 1.0f;
    light_on_[0] = true;

    for (int i = 1; i < light_num_max; i++) {
        light_position_[i][0] = 0.0f;
        light_position_[i][1] = -0.5f;
        light_position_[i][2] = 4.0f;
        light_position_[i][3] = 1.0f;
        light_type_[i] = LightType::PointLight;
        light_brightness_[i] = 1.0f;
        light_on_[i] = false;
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

    SubMenu* light_menu = new SubMenu("Light");
    menu.AddRootItem(light_menu);
	CommandMenuItem* light_on = new CommandMenuItem("Light On", [](std::vector<int>& menu_id) {
		Light::TurnOnLight(menu_id[1]-10);
	});
	int i_light_on=menu.AddItem(light_on);
	CommandMenuItem* light_off = new CommandMenuItem("Light Off", [](std::vector<int>& menu_id) {
		Light::TurnOffLight(menu_id[1]-10);
	});
	int i_light_off=menu.AddItem(light_off);
	CommandMenuItem* light_bri_up = new CommandMenuItem("Brightness Up", [](std::vector<int>& menu_id) {
		Light::AdjLightBrightness(menu_id[1]-10, 0.1f);
	});
	int i_light_bri_up=menu.AddItem(light_bri_up);
	CommandMenuItem* light_bri_down = new CommandMenuItem("Brightness Down", [](std::vector<int>& menu_id) {
		Light::AdjLightBrightness(menu_id[1]-10, -0.1f);
	});
	int i_light_bri_down=menu.AddItem(light_bri_down);
    CommandMenuItem* light_forwrrd = new CommandMenuItem("Move Forward", [](std::vector<int>& menu_id) {
        Light::MoveLightPosition(menu_id[1]-10, 0.0f, 0.0f, -1.0f, 0.0f);
    });
    int i_light_forward=menu.AddItem(light_forwrrd);
    CommandMenuItem* light_backward = new CommandMenuItem("Move Backward", [](std::vector<int>& menu_id) {
        Light::MoveLightPosition(menu_id[1]-10, 0.0f, 0.0f, 1.0f, 0.0f);
    });
    int i_light_backward=menu.AddItem(light_backward);
    CommandMenuItem* light_left = new CommandMenuItem("Move Left", [](std::vector<int>& menu_id) {
        Light::MoveLightPosition(menu_id[1]-10, -1.0f, 0.0f, 0.0f, 0.0f);
    });
    int i_light_left=menu.AddItem(light_left);
    CommandMenuItem* light_right = new CommandMenuItem("Move Right", [](std::vector<int>& menu_id) {
        Light::MoveLightPosition(menu_id[1]-10, 1.0f, 0.0f, 0.0f, 0.0f);
    });
    int i_light_right=menu.AddItem(light_right);

    SubMenu* light = new SubMenu("Light0");
    light->AddSubMenu(i_light_on);
    light->AddSubMenu(i_light_off);
    light->AddSubMenu(i_light_bri_up);
    light->AddSubMenu(i_light_bri_down);
    light_menu->AddSubMenu(menu.AddItem(light));
	for (int i=1; i < 8; i++) {
		SubMenu* light = new SubMenu("Light"+std::to_string(i));
		light->AddSubMenu(i_light_on);
		light->AddSubMenu(i_light_off);
		light->AddSubMenu(i_light_bri_up);
		light->AddSubMenu(i_light_bri_down);
        light->AddSubMenu(i_light_forward);
        light->AddSubMenu(i_light_backward);
        light->AddSubMenu(i_light_left);
        light->AddSubMenu(i_light_right);
		light_menu->AddSubMenu(menu.AddItem(light));
	}
}

void Light::FlushLight() {
    for (int light_id = 0; light_id < light_num_max; light_id++) {
        if (light_on_[light_id]) {
            if (light_type_[light_id] == LightType::PointLight) {
                glPushMatrix();
                glTranslatef(light_position_[light_id][0], light_position_[light_id][1], light_position_[light_id][2]);
                glutSolidCube(0.5);
                glPopMatrix();
            }
            glLightfv(GL_LIGHT0 + light_id, GL_POSITION, light_position_[light_id]);
            LightType type = light_type_[light_id];
            glLightfv(GL_LIGHT0 + light_id, GL_AMBIENT, coeff_4fv(light_ambient_[type], light_brightness_[light_id]));
            glLightfv(GL_LIGHT0 + light_id, GL_DIFFUSE, coeff_4fv(light_diffuse_[type], light_brightness_[light_id]));
            glLightfv(GL_LIGHT0 + light_id, GL_SPECULAR, coeff_4fv(light_specular_[type], light_brightness_[light_id]));
            glEnable(GL_LIGHT0 + light_id);
        }
        else {
            glDisable(GL_LIGHT0 + light_id);
        }
    }
}

void Light::TurnOnLight(int light_id) {
    light_on_[light_id] = true;
}

void Light::MoveLightPosition(int light_id, float x, float y, float z, float w) {
    light_position_[light_id][0] += x;
    light_position_[light_id][1] += y;
    light_position_[light_id][2] += z;
    light_position_[light_id][3] += w;
}

void Light::SetLightType(int light_id, enum LightType type) {
    light_type_[light_id] = type;
    light_position_[light_id][3] = (type == LightType::DirectionalLight) ? 0.0f : 1.0f;
}

void Light::AdjLightBrightness(int light_id, GLfloat dx) {
    if (light_brightness_[light_id] + dx > 0.0f && light_brightness_[light_id] + dx < 2.0f)
        light_brightness_[light_id] += dx;
}

void Light::TurnOffLight(int light_id) {
    light_on_[light_id] = false;
}