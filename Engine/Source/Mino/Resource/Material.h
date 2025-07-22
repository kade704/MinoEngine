#pragma once

#include "Shader.h"
#include "../Logger.h"
#include "../Serializable.h"
#include "../Resource/Texture.h"

#include <map>
#include <any>
#include <tinyxml2.h>

class Material : public Serializable
{
public:
	static Material* Create(const std::string& path);
	static bool Destroy(Material*& p_material);

public:
	void SetShader(Shader* shader);

	void FillUniform();

	void Bind(Texture* p_emptyTexture);
	void Unbind();

	template<typename T>
	inline void Set(const std::string p_key, const T& p_value)
	{
		if (HasShader())
		{
			if (m_uniformsData.find(p_key) != m_uniformsData.end())
				m_uniformsData[p_key] = std::any(p_value);
		}
		else
		{
			MINO_ERROR("Material Set failed: No attached shader");
		}
	}

	template<typename T>
	inline const T& Get(const std::string p_key)
	{
		if (m_uniformsData.find(p_key) != m_uniformsData.end())
			return T();
		else
			return std::any_cast<T>(m_uniformsData.at(p_key));
	}

	Shader* GetShader() const;
	bool HasShader() const;

	const std::map<std::string, std::any>& GetUniformsData() const;

	void SetDepthTest(bool p_depthTest);
	void SetBlendable(bool p_blendable);
	void SetBackfaceCulling(bool p_backfaceCulling);
	void SetFrontfaceCulling(bool p_frontfaceCulling);
	void SetGPUInstances(int p_instances);

	uint8_t GenerateStateMask() const;

	int GetGPUInstances() const;

	void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
	void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

	const std::string path;

private:
	Shader* m_shader;
	std::map<std::string, std::any> m_uniformsData;

	bool m_depthWriting = true;
	bool m_colorWriting = true;
	bool m_depthTest = true;
	bool m_backfaceCulling = true;
	bool m_frontfaceCulling = false;
	bool m_blendable = false;
	int m_gpuInstances = 1;
};

