#pragma once

#include <gl/glew.h>
#include <iostream>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "glm/glm.hpp"

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void ClearColor(const glm::vec4& clear_color) const;
    void DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawArrays(const unsigned count);
};

