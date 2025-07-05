#pragma once

#include <vector>
#include <string>

class Shader;

namespace Buffer
{
	class UniformBuffer
	{
	public:
		UniformBuffer(size_t p_size, uint32_t p_bindingPoint = 0, uint32_t p_offset = 0);
		~UniformBuffer();

		void Bind();
		void Unbind();

		template<typename T>
		void SetSubData(const T& p_data, size_t p_offset)
		{
			Bind();
			glBufferSubData(GL_UNIFORM_BUFFER, p_offset, sizeof(T), std::addressof(p_data));
			Unbind();
		}

		template<typename T>
		void SetSubData(const T& p_data, std::reference_wrapper<size_t> p_offsetInOut)
		{
			Bind();
			size_t dataSize = sizeof(T);
			glBufferSubData(GL_UNIFORM_BUFFER, p_offsetInOut.get(), dataSize, std::addressof(p_data));
			p_offsetInOut.get() += dataSize;
			Unbind();
		}

		uint32_t GetID() const;

		static void BindBlockToShader(Shader& p_shader, uint32_t p_uniformBlockLocation, uint32_t p_bindingPoint = 0);
		static void BindBlockToShader(Shader& p_shader, const std::string& p_name, uint32_t p_bindingPoint = 0);

		static uint32_t GetBlockLocation(Shader& p_shader, const std::string& p_name);

	private:
		uint32_t m_bufferID;
	};
}
