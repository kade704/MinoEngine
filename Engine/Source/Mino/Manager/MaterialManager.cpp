#include "MaterialManager.h"

Material* MaterialManager::CreateResource(const std::string& p_path)
{
	auto material = Material::Create(p_path);
	if (material)
	{
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(material) + offsetof(Material, path)) = p_path;
	}
	return material;
}

void MaterialManager::DestroyResource(Material* p_resource)
{
	Material::Destroy(p_resource);
}
