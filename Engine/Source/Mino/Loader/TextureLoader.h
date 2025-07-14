#pragma once

#include "../Resource/Texture.h"

#include <string>

class TextureLoader
{
public:
	TextureLoader() = delete;

	static Texture* Create(const std::string& path);
	static Texture* CreateFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height);
	static bool Destroy(Texture*& p_textureInstance);
};

