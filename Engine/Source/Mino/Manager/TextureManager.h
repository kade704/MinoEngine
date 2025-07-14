#pragma once

#include "../Loader/TextureLoader.h"
#include "AResourceManager.h"

class TextureManager : public AResourceManager<Texture>
{
public:
	virtual Texture* CreateResource(const std::string& p_path) override;
	virtual void DestroyResource(Texture* p_resource) override;
};