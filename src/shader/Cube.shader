#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(position, 1.0);
    Normal = vec3(model * vec4(normal, 1.0));
    FragPos = vec3(model * vec4(position, 1.0));
    v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 v_TexCoord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    normal = normalize(vec3(texture(material.normal, v_TexCoord)) * 2 - 0.5) + normal;
    normal = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //// combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    color = vec4(result, 1.0);
};