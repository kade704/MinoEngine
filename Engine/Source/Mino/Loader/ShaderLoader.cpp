#include <glad/glad.h>

#include "ShaderLoader.h"
#include "../Logger.h"


#include <fstream>
#include <sstream>
#include <vector>

Shader* ShaderLoader::Create(const std::string& path)
{
    std::pair<std::string, std::string> sources = ParseShader(path);

    unsigned int programID = CreateProgram(sources.first, sources.second);

    if (programID)
        return new Shader(path, programID);

    return nullptr;
}

Shader* ShaderLoader::CreateFromSource(const std::string& p_vertexShader, const std::string& p_fragmentShader)
{
    uint32_t programID = CreateProgram(p_vertexShader, p_fragmentShader);

    if (programID)
        return new Shader("", programID);

    return nullptr;
}

std::pair<std::string, std::string> ShaderLoader::ParseShader(const std::string& path)
{
    std::ifstream stream(path);

    if (!stream.is_open())
    {
        MINO_ERROR("Shader Not Found : " + path);
    }

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    std::string line;

    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos) type = ShaderType::VERTEX;
            if (line.find("fragment") != std::string::npos) type = ShaderType::FRAGMENT;
        }
        else if (type != ShaderType::NONE)
        {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return
    {
        ss[static_cast<int>(ShaderType::VERTEX)].str(),
        ss[static_cast<int>(ShaderType::FRAGMENT)].str(),
    };  
}

unsigned int ShaderLoader::CreateProgram(const std::string& vertexSource, const std::string& fragmentSource)
{
    unsigned int program = glCreateProgram();

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    if (vertexShader == 0 || fragmentShader == 0)
        return 0;

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        int maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::string infoLog(maxLength, ' ');
        glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

        MINO_ERROR("Shader Link Error \n" + infoLog);

        glDeleteProgram(program);

        return 0;
    }

    glValidateProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

unsigned int ShaderLoader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int shader = glCreateShader(type);

    const char* src = source.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        int maxLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::string infoLog(maxLength, ' ');
        glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

        MINO_ERROR("Shader Compile Error \n" + infoLog);

        glDeleteShader(shader);

        return 0;
    }

    return shader;
}

bool ShaderLoader::Destroy(Shader*& p_shader)
{
    if (p_shader)
    {
        delete p_shader;
        p_shader = nullptr;

        return true;
    }

    return false;
}
