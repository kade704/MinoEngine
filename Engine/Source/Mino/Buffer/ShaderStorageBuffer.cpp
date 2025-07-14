#include <glad/glad.h>

#include "ShaderStorageBuffer.h"

Buffer::ShaderStorageBuffer::ShaderStorageBuffer()
{
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_bufferID);
}

Buffer::ShaderStorageBuffer::~ShaderStorageBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
}

void Buffer::ShaderStorageBuffer::Bind(uint32_t p_bindingPoint)
{
	m_bindingPoint = p_bindingPoint;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, p_bindingPoint, m_bufferID);
}

void Buffer::ShaderStorageBuffer::Unbind()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, 0);
}
