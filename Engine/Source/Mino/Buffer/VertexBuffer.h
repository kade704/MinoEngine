#pragma once

#include <glad/glad.h>
#include <vector>

namespace Buffer
{
	template <class T>
	class VertexBuffer
	{
	public:
		VertexBuffer(T* p_data, size_t p_elements)
		{
			glGenBuffers(1, &m_bufferID);
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
			glBufferData(GL_ARRAY_BUFFER, p_elements * sizeof(T), p_data, GL_STATIC_DRAW);
		}

		VertexBuffer(std::vector<T>& p_data) :
			VertexBuffer(p_data.data(), p_data.size())
		{
		}

		~VertexBuffer()
		{
			glDeleteBuffers(1, &m_bufferID);
		}

		void Bind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		}

		void Unbind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		uint32_t GetID() const
		{
			return m_bufferID;
		}

	private:
		uint32_t m_bufferID;
	};
}