#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <signal.h>

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

#define ASSERT(x) \
    if (!(x))     \
    raise(SIGTRAP)
#define glCall(glFn) \
    glClearErrors(); \
    glFn;            \
    ASSERT(glLogCall(#glFn, __FILE__, __LINE__));

void glClearErrors();

bool glLogCall(const char *function, const char *file, unsigned int line);

class Renderer
{
private:
public:
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
    void Clear() const;
};

#endif // RENDERER_H