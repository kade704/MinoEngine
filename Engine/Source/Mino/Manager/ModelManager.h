#pragma once

#include "../Loader/ModelLoader.h"
#include "AResourceManager.h"

class ModelManager : public AResourceManager<Model>
{
public:
	virtual Model* CreateResource(const std::string& p_path) override;
	virtual void DestroyResource(Model* p_resource) override;
};

