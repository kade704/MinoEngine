#include "Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture* Texture::Create(const std::string& p_filepath, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
    GLuint textureID;
    int textureWidth;
    int textureHeight;
    int bitsPerPixel;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* dataBuffer = stbi_load(p_filepath.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);

    if (dataBuffer)
    {
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);

        if (p_generateMipmap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(p_firstFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(p_secondFilter));

        stbi_image_free(dataBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        return new Texture(p_filepath, textureID, textureWidth, textureHeight, bitsPerPixel, p_firstFilter, p_secondFilter, p_generateMipmap);
    }
    else
    {
        stbi_image_free(dataBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);
        return nullptr;
    }
}

Texture* Texture::CreateColor(uint32_t p_data, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &p_data);

    if (p_generateMipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(p_firstFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(p_secondFilter));

    glBindTexture(GL_TEXTURE_2D, 0);

    return new Texture("", textureID, 1, 1, 32, p_firstFilter, p_secondFilter, p_generateMipmap);
}

Texture* Texture::CreateFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p_data);

    if (p_generateMipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(p_firstFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(p_secondFilter));

    glBindTexture(GL_TEXTURE_2D, 0);

    return new Texture("", textureID, 1, 1, 32, p_firstFilter, p_secondFilter, p_generateMipmap);
}

bool Texture::Destroy(Texture*& p_textureInstance)
{
    if (p_textureInstance)
    {
        glDeleteTextures(1, &p_textureInstance->id);
        delete p_textureInstance;
        p_textureInstance = nullptr;
        return true;
    }

    return false;
}


void Texture::Bind(uint32_t p_slot) const
{
	glActiveTexture(GL_TEXTURE0 + p_slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string p_path, unsigned int p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bpp, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap) :
	path(p_path), id(p_id), width(p_width), height(p_height), bitsPerPixel(p_bpp), firstFilter(p_firstFilter), secondFilter(p_secondFilter), isMimapped(p_generateMipmap)
{
}