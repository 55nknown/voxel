#include <GL/glew.h>
#include <iostream>

#include "renderer.h"

void glClearErrors()
{
    while (glGetError());
}

bool glLogCall(const char *function, const char *file, unsigned int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") "
                  << function << " (" << file << ":" << line << ")" << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    glCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    glCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, NULL));

}