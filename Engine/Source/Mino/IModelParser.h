#pragma once

#include <string>

#include "Resource/Mesh.h"
#include "EModelParserFlags.h"

class IModelParser
{
public:
	virtual bool LoadModel
	(
		const std::string& p_fileName,
		std::vector<Mesh*>& p_meshes,
		std::vector<std::string>& p_materials,
		EModelParserFlags p_parserFlags
	) = 0;
};