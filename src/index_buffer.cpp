#include <GL/glew.h>

#include "index_buffer.h"
#include "renderer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : _count(count)
{
    glCall(glGenBuffers(1, &_id));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    glCall(glDeleteBuffers(1, &_id));
}

void IndexBuffer::Bind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, _id));
}

void IndexBuffer::Unbind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}