#pragma once

#include <vector>
#include <string>

#include "Mesh.h"

class ModelLoader;

class Model
{
	friend class ModelLoader;

public:
	const std::vector<Mesh*>& GetMeshes() const;
	const std::vector<std::string>& GetMaterialNames() const;

private:
	Model(const std::string& p_path);
	~Model();

public:
	std::string path;

private:
	std::vector<Mesh*> m_meshes;
	std::vector<std::string> m_materialNames;
};

