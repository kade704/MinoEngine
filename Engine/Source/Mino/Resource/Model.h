#pragma once

#include "Mesh.h"
#include "../Setting/EModelParserFlags.h"

#include <vector>
#include <string>

struct aiNode; 
struct aiMesh;
struct aiMaterial;
struct aiScene;

class Model
{
public:
	static Model* Create(const std::string& p_filepath, EModelParserFlags p_parserFlags);
	static void Reload(Model& p_model, const std::string& p_filePath, EModelParserFlags p_parserFlags);
	static bool Destroy(Model*& p_modelInstance);

private:
	static bool LoadModel(const std::string& p_fileName, std::vector<Mesh*>& p_meshes, std::vector<std::string>& p_materials, EModelParserFlags p_parserFlags);
	static void ProcessMaterials(const aiScene* p_scene, std::vector<std::string>& p_materials);
	static void ProcessNode(void* p_transform, aiNode* p_node, const aiScene* p_scene, std::vector<Mesh*>& p_meshes);
	static void ProcessMesh(void* p_transform, aiMesh* p_mesh, const aiScene* p_scene, std::vector<Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices);
	
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

