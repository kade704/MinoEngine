#pragma once

#include <string>

#include "Resource/Mesh.h"

class IModelParser
{
public:
	virtual bool LoadModel
	(
		const std::string& p_fileName,
		std::vector<Mesh*>& p_meshes,
		std::vector<std::string>& p_materials
	) = 0;
};