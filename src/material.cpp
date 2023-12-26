#include "material.h"
#include "vec_utils.h"

using namespace set_fv;

GLfloat Material::material_ambient_[MaterialType::Count][4];
GLfloat Material::material_diffuse_[MaterialType::Count][4];
GLfloat Material::material_specular_[MaterialType::Count][4];
GLfloat Material::material_shininess_[MaterialType::Count];

void Material::InitMaterial() {
    // 设置不同类型材质的效果

    // Arm
    set_4fv(material_ambient_[Arm], 0xc9811aU);
    set_4fv(material_diffuse_[Arm], 0xc9811aU);
    set_4fv(material_specular_[Arm], 0xc9811aU);
    material_shininess_[Arm] = 1.0f;

    // Base
    set_4fv(material_ambient_[Base], 0x400000U);
    set_4fv(material_diffuse_[Base], 0x400000U);
    set_4fv(material_specular_[Base], 0x400000U);
    material_shininess_[Base] = 1.0f;

    // Finger
    set_4fv(material_ambient_[Finger], 0x3b9be5U);
    set_4fv(material_diffuse_[Finger], 0x3b9be5U);
    set_4fv(material_specular_[Finger], 0x3b9be5U);
    material_shininess_[Finger] = 1.0f;

    // Box
    set_4fv(material_ambient_[Box], 0xd79b33U);
    set_4fv(material_diffuse_[Box], 0xd79b33U);
    set_4fv(material_specular_[Box], 0xd79b33U);
    material_shininess_[Box] = 0.0f;

    // Plate
    set_4fv(material_ambient_[Plate], 0x80b4eaU);
    set_4fv(material_diffuse_[Plate], 0x000000U);
    set_4fv(material_specular_[Plate], 0x000000U);
    material_shininess_[Plate] = 0.0f;

    // Desk
    set_4fv(material_ambient_[Desk], 0x101010U);
    set_4fv(material_diffuse_[Desk], 0x101010U);
    set_4fv(material_specular_[Desk], 0x737373U);
    material_shininess_[Desk] = 1.0f;

    // MouseBase
    set_4fv(material_ambient_[MouseBase], 0x50d050U);
    set_4fv(material_diffuse_[MouseBase], 0x503050U);
    set_4fv(material_specular_[MouseBase], 0x002000U);
    material_shininess_[MouseBase] = 1.0f;

    // MouseTip
    set_4fv(material_ambient_[MouseHead], 0x101010U);
    set_4fv(material_diffuse_[MouseHead], 0x505050U);
    set_4fv(material_specular_[MouseHead], 0xa0a0a0U);
    material_shininess_[MouseHead] = 0.8f;

    // Unknown
    set_4fv(material_ambient_[Unknown], 1.0f, 0.0f, 0.0f, 1.0f);
    set_4fv(material_diffuse_[Unknown], 1.0f, 0.0f, 0.0f, 1.0f);
    set_4fv(material_specular_[Unknown], 1.0f, 0.0f, 0.0f, 1.0f);
    material_shininess_[Unknown] = 0.0f;
}

void Material::SetMaterial(MaterialType type) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_[type]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_[type]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_[type]);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininess_[type]);
}