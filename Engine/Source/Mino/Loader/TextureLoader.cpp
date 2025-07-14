#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "TextureLoader.h"

Texture* TextureLoader::Create(const std::string& path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(true);

    int textureWidth;
    int textureHeight;
    int bitsPerPixel;
    unsigned char* bufferData = stbi_load(path.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);

    if (!bufferData)
    {
        stbi_image_free(bufferData);
        glBindTexture(GL_TEXTURE_2D, 0);

        return nullptr;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(bufferData);
    glBindTexture(GL_TEXTURE_2D, 0);

    return new Texture(path, textureID);
}

Texture* TextureLoader::CreateFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return new Texture("", textureID);
}

bool TextureLoader::Destroy(Texture*& p_textureInstance)
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
