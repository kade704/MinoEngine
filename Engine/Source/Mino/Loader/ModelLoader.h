#pragma once

#include "../Resource/Model.h"
#include "../AssimpParser.h"

class ModelLoader
{
public:
	ModelLoader() = delete;

	static Model* Create(const std::string& p_filepath);
	static void Reload(Model& p_model, const std::string& p_filePath);
	static bool Destroy(Model*& p_modelInstance);

private:
	static AssimpParser __ASSIMP;
};

