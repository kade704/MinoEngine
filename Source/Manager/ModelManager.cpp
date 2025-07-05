#include "ModelManager.h"

Model* ModelManager::CreateResource(const std::string& p_path)
{
	auto model = ModelLoader::Create(p_path);
	return model;
}

void ModelManager::DestroyResource(Model* p_resource)
{
	ModelLoader::Destroy(p_resource);
}
