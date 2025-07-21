#include "ModelManager.h"

Model* ModelManager::CreateResource(const std::string& p_path)
{
	auto model = ModelLoader::Create(p_path, 
		EModelParserFlags::TRIANGULATE | 
		EModelParserFlags::CALC_TANGENT_SPACE | 
		EModelParserFlags::GEN_SMOOTH_NORMALS | 
		EModelParserFlags::PRE_TRANSFORM_VERTICES);
	return model;
}

void ModelManager::DestroyResource(Model* p_resource)
{
	ModelLoader::Destroy(p_resource);
}
