#include "Model.h"

Model::Model(const std::string& p_path) :
    path(p_path)
{
}

Model::~Model()
{
}

const std::vector<Mesh*>& Model::GetMeshes() const
{
    return m_meshes;
}

const std::vector<std::string>& Model::GetMaterialNames() const
{
    return m_materialNames;
}