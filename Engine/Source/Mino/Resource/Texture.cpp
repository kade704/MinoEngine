#include <glad/glad.h>

#include "Texture.h"

Texture::Texture(const std::string p_path, unsigned int p_id) :
	path(p_path),
	id(p_id)
{
}

Texture::~Texture()
{
}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
