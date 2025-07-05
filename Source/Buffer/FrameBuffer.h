#pragma once

#include <vector>

#include "../Device.h"

namespace Buffer
{
	class FrameBuffer
	{
	public:
		FrameBuffer(uint16_t p_width = 0, uint16_t p_height = 0);
		~FrameBuffer();

		void Bind();
		void Unbind();

		void Resize(uint16_t width, uint16_t height);
		uint32_t GetTextureID() const;

	private:
		uint32_t m_bufferID;
		uint32_t m_renderTexture;
		uint32_t m_depthStencilBuffer;
	};
}
