#pragma once

#include "../Resource/Texture.h"
#include "../Setting/ETextureFilteringMode.h"

#include <string>

class TextureLoader
{
public:
	TextureLoader() = delete;

	static Texture* Create(const std::string& p_filepath, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap);
	static Texture* CreateColor(uint32_t p_data, ETextureFilteringMode p_firstFilter,ETextureFilteringMode p_secondFilter, bool p_generateMipmap);
	static Texture* CreateFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap);
	static bool Destroy(Texture*& p_textureInstance);
};

