#pragma once

#include "../Loader/ShaderLoader.h"
#include "AResourceManager.h"

class ShaderManager : public AResourceManager<Shader>
{
public:
	virtual Shader* CreateResource(const std::string& p_path) override;
	virtual void DestroyResource(Shader* p_resource) override;
};