#include <glad/glad.h>

#include "UniformBuffer.h"
#include "../Resource/Shader.h"

Buffer::UniformBuffer::UniformBuffer(size_t p_size, uint32_t p_bindingPoint, uint32_t p_offset)
{
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
	glBufferData(GL_UNIFORM_BUFFER, p_size, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, p_bindingPoint, m_bufferID, p_offset, p_size);
}

Buffer::UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
}

void Buffer::UniformBuffer::Bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
}

void Buffer::UniformBuffer::Unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

uint32_t Buffer::UniformBuffer::GetID() const
{
    return m_bufferID;
}

void Buffer::UniformBuffer::BindBlockToShader(Shader& p_shader, uint32_t p_uniformBlockLocation, uint32_t p_bindingPoint)
{
	glUniformBlockBinding(p_shader.id, p_uniformBlockLocation, p_bindingPoint);
}

void Buffer::UniformBuffer::BindBlockToShader(Shader& p_shader, const std::string& p_name, uint32_t p_bindingPoint)
{
	glUniformBlockBinding(p_shader.id, GetBlockLocation(p_shader, p_name), p_bindingPoint);
}

uint32_t Buffer::UniformBuffer::GetBlockLocation(Shader& p_shader, const std::string& p_name)
{
	return glGetUniformBlockIndex(p_shader.id, p_name.c_str());
}
