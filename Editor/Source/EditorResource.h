#pragma once

#include <unordered_map>
#include <string>

#include <Mino/Resource/Texture.h>
#include <Mino/Resource/Model.h>
#include <Mino/Resource/Shader.h>

class EditorResource
{
public:
	EditorResource();

	Texture* GetFileIcon(const std::string& p_filename);
	Texture* GetTexture(const std::string& p_id);
	Model* GetModel(const std::string& p_id);
	Shader* GetShader(const std::string& p_id);

private:
	std::unordered_map<std::string, Texture*> m_textures;
	std::unordered_map<std::string, Model*> m_models;
	std::unordered_map<std::string, Shader*> m_shaders;
};

