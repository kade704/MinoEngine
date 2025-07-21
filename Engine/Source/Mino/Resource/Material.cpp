#include "Material.h"
#include "../Serializer.h"
#include "../UniformInfo.h"
#include "../Logger.h"
#include "../Buffer/UniformBuffer.h"

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

void Material::Bind(Texture* p_emptyTexture)
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
		auto uniformData = m_shader->GetUniformInfo(name);

		if (uniformData)
		{
			switch (uniformData->type)
			{
			case UniformType::UNIFORM_BOOL:			if (value.type() == typeid(bool))		m_shader->SetUniformInt(name, std::any_cast<bool>(value));			break;
			case UniformType::UNIFORM_INT:			if (value.type() == typeid(int))		m_shader->SetUniformInt(name, std::any_cast<int>(value));			break;
			case UniformType::UNIFORM_FLOAT:		if (value.type() == typeid(float))		m_shader->SetUniformFloat(name, std::any_cast<float>(value));		break;
			case UniformType::UNIFORM_FLOAT_VEC2:	if (value.type() == typeid(FVector2))	m_shader->SetUniformVec2(name, std::any_cast<FVector2>(value));		break;
			case UniformType::UNIFORM_FLOAT_VEC3:	if (value.type() == typeid(FVector3))	m_shader->SetUniformVec3(name, std::any_cast<FVector3>(value));		break;
			case UniformType::UNIFORM_FLOAT_VEC4:	if (value.type() == typeid(FVector4))	m_shader->SetUniformVec4(name, std::any_cast<FVector4>(value));		break;
			case UniformType::UNIFORM_SAMPLER_2D:
				{
					if (value.type() == typeid(Texture*))
					{
						if (auto tex = std::any_cast<Texture*>(value); tex)
						{
							tex->Bind(textureSlot);
							m_shader->SetUniformInt(uniformData->name, textureSlot++);
						}
						else if (p_emptyTexture)
						{
							p_emptyTexture->Bind(textureSlot);
							m_shader->SetUniformInt(uniformData->name, textureSlot++);
						}
					}
				}
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

void Material::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Serializer::SerializeShader(p_doc, p_node, "shader", m_shader);

	tinyxml2::XMLNode* uniformsNode = p_doc.NewElement("uniforms");
	p_node->InsertEndChild(uniformsNode);

	for (const auto& [uniformName, uniformValue] : m_uniformsData)
	{
		tinyxml2::XMLNode* uniform = p_doc.NewElement("uniform");
		uniformsNode->InsertEndChild(uniform); 

		const UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

		Serializer::SerializeString(p_doc, uniform, "name", uniformName);

		if (uniformInfo && uniformValue.has_value())
		{
			switch (uniformInfo->type)
			{
			case UniformType::UNIFORM_BOOL:
				if (uniformValue.type() == typeid(bool)) Serializer::SerializeInt(p_doc, uniform, "value", std::any_cast<bool>(uniformValue));
				break;

			case UniformType::UNIFORM_INT:
				if (uniformValue.type() == typeid(int)) Serializer::SerializeInt(p_doc, uniform, "value", std::any_cast<int>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT:
				if (uniformValue.type() == typeid(float)) Serializer::SerializeFloat(p_doc, uniform, "value", std::any_cast<float>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT_VEC2:
				if (uniformValue.type() == typeid(FVector2)) Serializer::SerializeVec2(p_doc, uniform, "value", std::any_cast<FVector2>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT_VEC3:
				if (uniformValue.type() == typeid(FVector3)) Serializer::SerializeVec3(p_doc, uniform, "value", std::any_cast<FVector3>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT_VEC4:
				if (uniformValue.type() == typeid(FVector4)) Serializer::SerializeVec4(p_doc, uniform, "value", std::any_cast<FVector4>(uniformValue));
				break;

			case UniformType::UNIFORM_SAMPLER_2D:
				if (uniformValue.type() == typeid(Texture*)) Serializer::SerializeTexture(p_doc, uniform, "value", std::any_cast<Texture*>(uniformValue));
				break;
			}
		}
	}
}

void Material::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Shader* deserializedShader = Serializer::DeserializeShader(p_doc, p_node, "shader");
	if (deserializedShader)
	{
		SetShader(deserializedShader);

		tinyxml2::XMLNode* uniformsNode = p_node->FirstChildElement("uniforms");

		if (uniformsNode)
		{
			for (auto uniform = uniformsNode->FirstChildElement("uniform"); uniform; uniform = uniform->NextSiblingElement("uniform"))
			{
				if (auto uniformNameElement = uniform->FirstChildElement("name"); uniformNameElement)
				{
					const std::string uniformName = uniformNameElement->GetText();

					UniformInfo* uniformInfo = deserializedShader->GetUniformInfo(uniformName);

					switch (uniformInfo->type)
					{
					case UniformType::UNIFORM_BOOL:
						m_uniformsData[uniformInfo->name] = Serializer::DeserializeBoolean(p_doc, uniform, "value");
						break;
					case UniformType::UNIFORM_INT:
						m_uniformsData[uniformName] = Serializer::DeserializeInt(p_doc, uniform, "value");
						break;
					case UniformType::UNIFORM_FLOAT:
						m_uniformsData[uniformName] = Serializer::DeserializeFloat(p_doc, uniform, "value");
						break;
					case UniformType::UNIFORM_FLOAT_VEC2:
						m_uniformsData[uniformName] = Serializer::DeserializeVec2(p_doc, uniform, "value");
						break;
					case UniformType::UNIFORM_FLOAT_VEC3:
						m_uniformsData[uniformName] = Serializer::DeserializeVec3(p_doc, uniform, "value");
						break;
					case UniformType::UNIFORM_FLOAT_VEC4:
						m_uniformsData[uniformName] = Serializer::DeserializeVec4(p_doc, uniform, "value");
						break;
					case UniformType::UNIFORM_SAMPLER_2D:
						m_uniformsData[uniformName] = Serializer::DeserializeTexture(p_doc, uniform, "value");
						break;
					}
				}
			}
		}
	}
}