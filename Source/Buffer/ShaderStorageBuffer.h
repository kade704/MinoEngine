#pragma once

#include <vector>

namespace Buffer
{
	class ShaderStorageBuffer
	{
	public:
		ShaderStorageBuffer();
		~ShaderStorageBuffer();

		void Bind(uint32_t p_bindingPoint);
		void Unbind();

		template<typename T>
		void SendBlocks(T* p_data, size_t p_size)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
			glBufferData(GL_SHADER_STORAGE_BUFFER, p_size, p_data, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

	private:
		uint32_t m_bufferID;
		uint32_t m_bindingPoint = 0;
	};
}
