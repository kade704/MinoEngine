#include "ShaderManager.h"

Shader* ShaderManager::CreateResource(const std::string& p_path)
{
	auto shader = Shader::Create(p_path);
	return shader;
}

void ShaderManager::DestroyResource(Shader* p_resource)
{
	Shader::Destroy(p_resource);
}
