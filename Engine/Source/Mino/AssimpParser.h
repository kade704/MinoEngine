#pragma once

#include "IModelParser.h"
#include "Vertex.h"

class AssimpParser : public IModelParser
{
public:
	bool LoadModel
	(
		const std::string& p_fileName,
		std::vector<Mesh*>& p_meshes,
		std::vector<std::string>& p_materials
	) override;

private:
	void ProcessMaterials(const struct aiScene* p_scene, std::vector<std::string>& p_materials);;
	void ProcessNode(void* p_transform, struct aiNode* p_node, const struct aiScene* p_scene, std::vector<Mesh*>& p_meshes);
	void ProcessMesh(void* p_transform, struct aiMesh* p_mesh, const struct aiScene* p_scene, std::vector<Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices);
};