#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>

#include <AssimpParser.h>

bool AssimpParser::LoadModel(const std::string& p_fileName, std::vector<Mesh*>& p_meshes, std::vector<std::string>& p_materials)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(p_fileName, 
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals | 
		aiProcess_OptimizeMeshes | 
		aiProcess_CalcTangentSpace | 
		// aiProcess_JoinIdenticalVertices |
		// aiProcess_Debone |
		aiProcess_FindInvalidData |
		// aiProcess_ImproveCacheLocality |
		// aiProcess_GenUVCoords |
		aiProcess_PreTransformVertices |
		aiProcess_GlobalScale
	);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		return false;

	ProcessMaterials(scene, p_materials);

	aiMatrix4x4 identity;

	ProcessNode(&identity, scene->mRootNode, scene, p_meshes);

	return true;
}

void AssimpParser::ProcessMaterials(const aiScene* p_scene, std::vector<std::string>& p_materials)
{
	for (uint32_t i = 0; i < p_scene->mNumMaterials; ++i)
	{
		aiMaterial* material = p_scene->mMaterials[i];
		if (material)
		{
			aiString name;
			aiGetMaterialString(material, AI_MATKEY_NAME, &name);
			p_materials.push_back(name.C_Str());
		}
	}
}

void AssimpParser::ProcessNode(void* p_transform, aiNode* p_node, const aiScene* p_scene, std::vector<Mesh*>& p_meshes)
{
	aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(p_transform) * p_node->mTransformation;

	for (uint32_t i = 0; i < p_node->mNumMeshes; ++i)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
		ProcessMesh(&nodeTransformation, mesh, p_scene, vertices, indices);
		p_meshes.push_back(new Mesh(vertices, indices, mesh->mMaterialIndex));
	}

	for (uint32_t i = 0; i < p_node->mNumChildren; ++i)
	{
		ProcessNode(&nodeTransformation, p_node->mChildren[i], p_scene, p_meshes);
	}
}

void AssimpParser::ProcessMesh(void* p_transform, aiMesh* p_mesh, const aiScene* p_scene, std::vector<Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices)
{
	aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(p_transform);

	for (uint32_t i = 0; i < p_mesh->mNumVertices; ++i)
	{
		aiVector3D position = meshTransformation * p_mesh->mVertices[i];
		aiVector3D normal = meshTransformation * (p_mesh->mNormals ? p_mesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f));
		aiVector3D texCoords = p_mesh->mTextureCoords[0] ? p_mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
		aiVector3D tangent = p_mesh->mTangents ? meshTransformation * p_mesh->mTangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
		aiVector3D bitangent = p_mesh->mBitangents ? meshTransformation * p_mesh->mBitangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);

		p_outVertices.push_back
		(
			{
				position.x,
				position.y,
				position.z,
				texCoords.x,
				texCoords.y,
				normal.x,
				normal.y,
				normal.z,
				tangent.x,
				tangent.y,
				tangent.z,
				bitangent.x,
				bitangent.y,
				bitangent.z
			}
		);
	}

	for (uint32_t faceID = 0; faceID < p_mesh->mNumFaces; ++faceID)
	{
		auto& face = p_mesh->mFaces[faceID];

		for (size_t indexID = 0; indexID < 3; ++indexID)
			p_outIndices.push_back(face.mIndices[indexID]);
	}
}
