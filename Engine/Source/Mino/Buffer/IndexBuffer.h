#pragma once
#include <vector>

namespace Buffer
{
	class IndexBuffer
	{
	public:
		IndexBuffer(unsigned int* p_data, size_t p_elements);
		IndexBuffer(std::vector<uint32_t>& p_data);
		~IndexBuffer();

		void Bind();
		void Unbind();

		uint32_t GetID();

	private:
		uint32_t m_bufferID;
	};
}
