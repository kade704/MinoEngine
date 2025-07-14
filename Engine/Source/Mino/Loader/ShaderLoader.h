#pragma once

#include "../Resource/Shader.h"

#include <string>

class ShaderLoader
{
public:
	ShaderLoader() = delete;

	static Shader* Create(const std::string& path);
	static Shader* CreateFromSource(const std::string& p_vertexShader, const std::string& p_fragmentShader);
	static bool Destroy(Shader*& p_shader);

private:
	static std::pair<std::string, std::string> ParseShader(const std::string& path);
	static unsigned int CreateProgram(const std::string& vertexSource, const std::string& fragmentSource);
	static unsigned int CompileShader(unsigned int type, const std::string& source);
	
};

