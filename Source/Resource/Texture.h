#pragma once

#include <stdint.h>
#include <string>

class Texture
{
	friend class TextureLoader;

private:
	Texture(const std::string p_path, unsigned int p_id);
	~Texture();

public:
	void Bind(unsigned int slot);
	void Unbind();

public:
	const std::string path;
	const uint32_t id;
};

