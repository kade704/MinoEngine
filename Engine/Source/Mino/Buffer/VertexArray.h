#pragma once

#include "VertexBuffer.h"

namespace Buffer
{
	enum class EType
	{
		BYTE = 0x1400,
		UNISGNED_BYTE = 0x1401,
		SHORT = 0x1402,
		UNSIGNED_SHORT = 0x1403,
		INT = 0x1404,
		UNSIGNED_INT = 0x1405,
		FLOAT = 0x1406,
		DOUBLE = 0x140A
	};

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		template <class T>
		void BindAttribute(uint32_t p_attribute, VertexBuffer<T>& p_vertexBuffer, EType p_type, uint64_t p_count, uint64_t p_stride, intptr_t p_offset)
		{
			Bind();
			p_vertexBuffer.Bind();
			glEnableVertexAttribArray(p_attribute);
			glVertexAttribPointer(static_cast<GLuint>(p_attribute), static_cast<GLint>(p_count), static_cast<GLenum>(p_type), GL_FALSE, static_cast<GLsizei>(p_stride), reinterpret_cast<const GLvoid*>(p_offset));
		}

		void Bind();
		void Unbind();
		GLint GetID();

	private:
		GLuint m_bufferID;
	};
}