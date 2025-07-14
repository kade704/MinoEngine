#include "ShaderManager.h"

Shader* ShaderManager::CreateResource(const std::string& p_path)
{
	auto shader = ShaderLoader::Create(p_path);
	return shader;
}

void ShaderManager::DestroyResource(Shader* p_resource)
{
	ShaderLoader::Destroy(p_resource);
}
