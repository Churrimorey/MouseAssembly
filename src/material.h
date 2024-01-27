#pragma once
#include <gl/glut.h>

class Material {
public:
    enum MaterialType {
        Arm = 0,
        Base,
        Finger,
        MouseBase,
        MouseHead,
        Box,
        Plate,
        Desk,
        Battery,
        UIGreen,
        Unknown,
        Count
    };

private:
    static GLfloat material_ambient_[MaterialType::Count][4];
    static GLfloat material_diffuse_[MaterialType::Count][4];
    static GLfloat material_specular_[MaterialType::Count][4];
    static GLfloat material_shininess_[MaterialType::Count];

public:
    static void InitMaterial();
    static void SetMaterial(MaterialType type);
    static void SetColor(float R, float G, float B);
};
