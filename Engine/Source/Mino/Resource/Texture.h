#pragma once

#include "../Setting/ETextureFilteringMode.h"

#include <stdint.h>
#include <string>

class Texture
{
	friend class TextureLoader;

private:
	Texture(const std::string p_path, uint32_t p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bpp, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap);
	~Texture() = default;

public:
	void Bind(uint32_t p_slot) const;
	void Unbind();

public:
	const uint32_t id;
	const uint32_t width;
	const uint32_t height;
	const uint32_t bitsPerPixel;
	const ETextureFilteringMode firstFilter;
	const ETextureFilteringMode secondFilter;
	const std::string path;
	const bool isMimapped;
};

