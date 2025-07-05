#include "TextureManager.h"

Texture* TextureManager::CreateResource(const std::string& p_path)
{
	auto texture = TextureLoader::Create(p_path);
	return texture;
}

void TextureManager::DestroyResource(Texture* p_resource)
{
	TextureLoader::Destroy(p_resource);
}
