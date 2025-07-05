#include <glad/glad.h>

#include "Shader.h"
#include "../Logger.h"
#include "Texture.h"

Shader::Shader(const std::string p_path, unsigned int id) :
	path(p_path),
	id(id)
{
	QueryUniforms();
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::Bind() const
{
	glUseProgram(id);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

UniformInfo* Shader::GetUniformInfo(const std::string& name)
{
	auto found = std::find_if(uniforms.begin(), uniforms.end(), [&name](const UniformInfo& element)
	{
		return name == element.name;
	});

	if (found != uniforms.end())
		return &*found;
	else
		return nullptr;
}

int Shader::GetUniformInt(const std::string& name)
{
	int value;
	glGetUniformiv(id, GetUniformLocation(name), &value);
	return value;
}

float Shader::GetUniformFloat(const std::string& name)
{
	float value;
	glGetUniformfv(id, GetUniformLocation(name), &value);
	return value;
}

FVector2 Shader::GetUniformVec2(const std::string& name)
{
	GLfloat values[2];
	glGetUniformfv(id, GetUniformLocation(name), values);
	return reinterpret_cast<FVector2&>(values);
}

FVector3 Shader::GetUniformVec3(const std::string& name)
{
	GLfloat values[3];
	glGetUniformfv(id, GetUniformLocation(name), values);
	return reinterpret_cast<FVector3&>(values);
}

FVector4 Shader::GetUniformVec4(const std::string& name)
{
	GLfloat values[4];
	glGetUniformfv(id, GetUniformLocation(name), values);
	return reinterpret_cast<FVector4&>(values);
}

FMatrix4 Shader::GetUniformMat4(const std::string& name)
{
	GLfloat values[16];
	glGetUniformfv(id, GetUniformLocation(name), values);
	return reinterpret_cast<FMatrix4&>(values);
}

void Shader::SetUniformInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformFloat(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformVec2(const std::string& name, const FVector2& p_vec2)
{
	glUniform2f(GetUniformLocation(name), p_vec2.x, p_vec2.y);
}

void Shader::SetUniformVec3(const std::string& name, const FVector3& p_vec3)
{
	glUniform3f(GetUniformLocation(name), p_vec3.x, p_vec3.y, p_vec3.z);
}

void Shader::SetUniformVec4(const std::string& name, const FVector4& p_vec4)
{
	glUniform4f(GetUniformLocation(name), p_vec4.x, p_vec4.y, p_vec4.z, p_vec4.w);
}

void Shader::SetUniformMat4(const std::string& name, const FMatrix4& p_mat4)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &p_mat4.data[0]);
}

bool Shader::IsEngineUBOMember(const std::string& p_uniformName)
{
	return p_uniformName.rfind("ubo_", 0) == 0;
}

void Shader::QueryUniforms()
{
	int numActiveUniforms;
	uniforms.clear();
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
	std::vector<char> nameData(256);
	for (int i = 0; i < numActiveUniforms; i++)
	{
		int arraySize;
		unsigned int type;
		int actualLength;
		glGetActiveUniform(id, i, 256, &actualLength, &arraySize, &type, &nameData[0]);
		std::string name(nameData.data(), actualLength);

		if (!IsEngineUBOMember(name))
		{
			std::any defaultValue;

			switch (static_cast<UniformType>(type))
			{
			case UniformType::UNIFORM_INT:			defaultValue = std::make_any<int>(GetUniformInt(name));	break;
			case UniformType::UNIFORM_FLOAT:		defaultValue = std::make_any<float>(GetUniformFloat(name));	break;
			case UniformType::UNIFORM_VEC2:	defaultValue = std::make_any<FVector2>(GetUniformVec2(name)); break;
			case UniformType::UNIFORM_VEC3:	defaultValue = std::make_any<FVector3>(GetUniformVec3(name)); break;
			case UniformType::UNIFORM_VEC4:	defaultValue = std::make_any<FVector4>(GetUniformVec4(name)); break;
			case UniformType::UNIFORM_SAMPLER_2D:	defaultValue = std::make_any<Texture*>(nullptr); break;
			}

			uniforms.push_back
			({
				static_cast<UniformType>(type),
				name,
				GetUniformLocation(name),
				defaultValue
				});
		}
	}
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
	{
		return mUniformLocationCache.at(name);
	}

	int location = glGetUniformLocation(id, name.c_str());

	if (location == -1)
	{
		MINO_WARNING("Uniform: " + name + " doesn't exist ");
	}

	mUniformLocationCache[name] = location;

	return location;
}
