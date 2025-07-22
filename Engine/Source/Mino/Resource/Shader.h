#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../UniformInfo.h"
#include "../Math/FVector2.h"
#include "../Math/FVector3.h"
#include "../Math/FVector4.h"
#include "../Math/FMatrix4.h"

class Shader
{
public:
	static Shader* Create(const std::string& path);
	static Shader* CreateFromSource(const std::string& p_vertexShader, const std::string& p_fragmentShader);
	static bool Destroy(Shader*& p_shader);

private:
	static std::pair<std::string, std::string> ParseShader(const std::string& path);
	static unsigned int CreateProgram(const std::string& vertexSource, const std::string& fragmentSource);
	static unsigned int CompileShader(unsigned int type, const std::string& source);

private:
	Shader(const std::string p_path, unsigned int id);
	~Shader();

public:
	void Bind() const;
	void Unbind() const;

	UniformInfo* GetUniformInfo(const std::string& name);

	int GetUniformInt(const std::string& name);
	float GetUniformFloat(const std::string& name);
	FVector2 GetUniformVec2(const std::string& name);
	FVector3 GetUniformVec3(const std::string& name);
	FVector4 GetUniformVec4(const std::string& name);
	FMatrix4 GetUniformMat4(const std::string& name);

	void SetUniformInt(const std::string& name, int value);
	void SetUniformFloat(const std::string& name, float value);
	void SetUniformVec2(const std::string& name, const FVector2& p_vec2);
	void SetUniformVec3(const std::string& name, const FVector3& p_vec3);
	void SetUniformVec4(const std::string& name, const FVector4& p_vec4);
	void SetUniformMat4(const std::string& name, const FMatrix4& p_mat4);

private:
	bool IsEngineUBOMember(const std::string& p_uniformName);
	void QueryUniforms();
	unsigned int GetUniformLocation(const std::string& name);

public:
	const std::string path;
	unsigned int id;
	std::vector<UniformInfo> uniforms;

private:
	std::unordered_map<std::string, int> m_uniformLocationCache;
};