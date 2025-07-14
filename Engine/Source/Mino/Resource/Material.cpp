#include "Material.h"
#include "../Serializer.h"
#include "../UniformInfo.h"
#include "../Logger.h"
#include "../Buffer/UniformBuffer.h"

void Material::OnDeserialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node)
{
	Shader* deserializedShader = Serializer::DeserializeShader(doc, node, "shader");
	SetShader(deserializedShader);

	tinyxml2::XMLNode* uniformsNode = node->FirstChildElement("uniforms");
	if (!uniformsNode) return;
	
	tinyxml2::XMLNode* uniformNode = uniformsNode->FirstChildElement("uniform");
	while (uniformNode)
	{
		std::string uniformName = Serializer::DeserializeString(doc, uniformNode, "name");
			 
		UniformInfo* uniformInfo = deserializedShader->GetUniformInfo(uniformName);

		switch (uniformInfo->type)
		{
		case UniformType::UNIFORM_INT:
			m_uniformsData[uniformName] = Serializer::DeserializeInt64(doc, uniformNode, "value");
			break;
		case UniformType::UNIFORM_FLOAT:
			m_uniformsData[uniformName] = Serializer::DeserializeFloat(doc, uniformNode, "value");
			break;
		case UniformType::UNIFORM_VEC2:
			m_uniformsData[uniformName] = Serializer::DeserializeVec2(doc, uniformNode, "value");
			break;
		case UniformType::UNIFORM_VEC3:
			m_uniformsData[uniformName] = Serializer::DeserializeVec3(doc, uniformNode, "value");
			break;
		case UniformType::UNIFORM_VEC4:
			m_uniformsData[uniformName] = Serializer::DeserializeVec4(doc, uniformNode, "value");
			break;
		case UniformType::UNIFORM_MAT4:
			m_uniformsData[uniformName] = Serializer::DeserializeMat4(doc, uniformNode, "value");
			break;

		case UniformType::UNIFORM_SAMPLER_2D:
			m_uniformsData[uniformName] = Serializer::DeserializeTexture(doc, uniformNode, "value");
			break;

		default:
			break;
		}

		uniformNode = uniformNode->NextSiblingElement("uniform");
	}
}

Shader* Material::GetShader() const
{
	return m_shader;
}

void Material::SetShader(Shader* shader)
{
	m_shader = shader;
	if (m_shader)
	{
		Buffer::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
		FillUniform();
	}
	else
	{
		m_uniformsData.clear();
	}
}

bool Material::HasShader() const
{
	return m_shader != nullptr;
}

const std::map<std::string, std::any>& Material::GetUniformsData() const
{
	return m_uniformsData;
}

void Material::Bind()
{
	if (!HasShader())
	{
		MINO_WARNING("material has no shader");
		return;
	}

	m_shader->Bind();

	int textureSlot = 0;

	for (auto& [name, value] : m_uniformsData)
	{
		UniformInfo* uniformInfo = m_shader->GetUniformInfo(name);

		if (uniformInfo)
		{
			switch (uniformInfo->type)
			{
			case UniformType::UNIFORM_INT:
				m_shader->SetUniformInt(name, std::any_cast<int>(value));
				break;
			case UniformType::UNIFORM_FLOAT:
				m_shader->SetUniformFloat(name, std::any_cast<float>(value));
				break;
			case UniformType::UNIFORM_VEC2:
				m_shader->SetUniformVec2(name, std::any_cast<FVector2>(value));
				break;
			case UniformType::UNIFORM_VEC3:
				m_shader->SetUniformVec3(name, std::any_cast<FVector3>(value));
				break;
			case UniformType::UNIFORM_VEC4:
				m_shader->SetUniformVec4(name, std::any_cast<FVector4>(value));
				break;
			case UniformType::UNIFORM_MAT4:
				m_shader->SetUniformMat4(name, std::any_cast<FMatrix4>(value));
				break;
			case UniformType::UNIFORM_SAMPLER_2D:
				Texture* tex = std::any_cast<Texture*>(value);
				if (tex)
				{
					tex->Bind(textureSlot);
					m_shader->SetUniformInt(name, std::any_cast<int>(textureSlot));
					textureSlot++;
				}
				break;
			}
		}
	}
}

void Material::Unbind()
{
	if (HasShader())
		m_shader->Unbind();
}

void Material::FillUniform()
{
	m_uniformsData.clear();

	for (const UniformInfo& element : m_shader->uniforms)
		m_uniformsData.emplace(element.name, element.defaultValue);
}

void Material::SetDepthTest(bool p_depthTest)
{
	m_depthTest = p_depthTest;
}

void Material::SetBlendable(bool p_blendable)
{
	m_blendable = p_blendable;
}

void Material::SetBackfaceCulling(bool p_backfaceCulling)
{
	m_backfaceCulling = p_backfaceCulling;
}

void Material::SetFrontfaceCulling(bool p_frontfaceCulling)
{
	m_frontfaceCulling = p_frontfaceCulling;
}

void Material::SetGPUInstances(int p_instances)
{
	m_gpuInstances = p_instances;
}

uint8_t Material::GenerateStateMask() const
{
	uint8_t result = 0;

	if (m_depthWriting)								result |= 0b0000'0001;
	if (m_colorWriting)								result |= 0b0000'0010;
	if (m_blendable)								result |= 0b0000'0100;
	if (m_backfaceCulling || m_frontfaceCulling)	result |= 0b0000'1000;
	if (m_depthTest)								result |= 0b0001'0000;
	if (m_backfaceCulling)							result |= 0b0010'0000;
	if (m_frontfaceCulling)							result |= 0b0100'0000;

	return result;
}

int Material::GetGPUInstances() const
{
	return m_gpuInstances;
}
