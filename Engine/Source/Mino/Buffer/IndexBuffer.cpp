#include <glad/glad.h>

#include "IndexBuffer.h"

Buffer::IndexBuffer::IndexBuffer(unsigned int* p_data, size_t p_elements)
{
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_elements * sizeof(unsigned int), p_data, GL_STATIC_DRAW);
}

Buffer::IndexBuffer::IndexBuffer(std::vector<uint32_t>& p_data) :
	IndexBuffer(p_data.data(), p_data.size())
{
}

Buffer::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
}

void Buffer::IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
}

void Buffer::IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t Buffer::IndexBuffer::GetID()
{
    return m_bufferID;
}
