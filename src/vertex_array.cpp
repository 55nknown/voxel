#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include "renderer.h"

VertexArray::VertexArray()
{
    glCall(glGenVertexArrays(1, &_id));
}

VertexArray::~VertexArray()
{
    glCall(glDeleteVertexArrays(1, &_id));
}

void VertexArray::Bind() const
{
    glCall(glBindVertexArray(_id));
}

void VertexArray::Unbind() const
{
    glCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    const auto &elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        glCall(glEnableVertexAttribArray(i));
        glCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}