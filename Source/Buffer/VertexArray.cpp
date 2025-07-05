#include "VertexArray.h"

Buffer::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_bufferID);
    glBindVertexArray(m_bufferID);
}

Buffer::VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_bufferID);
}

void Buffer::VertexArray::Bind()
{
    glBindVertexArray(m_bufferID);
}

void Buffer::VertexArray::Unbind()
{
    glBindVertexArray(0);
}

GLint Buffer::VertexArray::GetID()
{
    return m_bufferID;
}
