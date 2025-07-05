#pragma once

#include <string>

class PathParser
{
public:
	PathParser() = delete;

	enum class EFileType
	{
		UNKNOWN,
		MODEL,
		TEXTURE,
		SHADER,
		MATERIAL,
		SOUND,
		SCENE,
		SCRIPT,
		FONT
	};

	static std::string GetExtension(const std::string& p_path);
	static std::string GetElementName(const std::string& p_path);
	static std::string GetContainingFolder(const std::string& p_path);
	static std::string FileTypeToString(EFileType p_fileType);
	static EFileType GetFileType(const std::string& p_path);
};

