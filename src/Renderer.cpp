#include "renderer.h"
#include <iostream>
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << function <<
            " " << file << ":" << line <<
            std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawArrays(const unsigned count)
{
    glDrawArrays(GL_TRIANGLES, 0, count);
}

void Renderer::ClearColor(const glm::vec4& clear_color) const
{
    GLCall(glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a ));
}
