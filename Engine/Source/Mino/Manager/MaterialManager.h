#pragma once

#include "../Resource/Material.h"
#include "AResourceManager.h"

class MaterialManager : public AResourceManager<Material>
{
public:
	virtual Material* CreateResource(const std::string& p_path) override;
	virtual void DestroyResource(Material* p_resource) override;
};