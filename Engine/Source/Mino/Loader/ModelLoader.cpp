#include "ModelLoader.h"

AssimpParser ModelLoader::__ASSIMP;

Model* ModelLoader::Create(const std::string& p_filepath, EModelParserFlags p_parserFlags)
{
	Model* result = new Model(p_filepath);

	if (__ASSIMP.LoadModel(p_filepath, result->m_meshes, result->m_materialNames, p_parserFlags))
	{
		return result;
	}

	delete result;

	return nullptr;
}

void ModelLoader::Reload(Model& p_model, const std::string& p_filePath, EModelParserFlags p_parserFlags)
{
	Model* newModel = Create(p_filePath, p_parserFlags);

	if (newModel)
	{
		p_model.m_meshes = newModel->m_meshes;
		p_model.m_materialNames = newModel->m_materialNames;
		newModel->m_meshes.clear();
		delete newModel;
	}
}

bool ModelLoader::Destroy(Model*& p_modelInstance)
{
	if (p_modelInstance)
	{
		delete p_modelInstance;
		p_modelInstance = nullptr;

		return true;
	}

	return false;
}
