#pragma once

#include "../Resource/Material.h"

#include <string>

class MaterialLoader
{
public:
	MaterialLoader() = delete;

	static Material* Create(const std::string& path);
	static bool Destroy(Material*& p_material);
};

