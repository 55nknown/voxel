#include <GL/glew.h>

#include "vertex_buffer.h"
#include "renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    glCall(glGenBuffers(1, &_id));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, _id));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    glCall(glDeleteBuffers(1, &_id));
}

void VertexBuffer::Bind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, _id));
}

void VertexBuffer::Unbind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}