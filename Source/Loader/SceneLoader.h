#pragma once

#include "../Scene.h"

#include <string>

class SceneLoader
{
public:
	static Scene* Create(const std::string& path);
};

