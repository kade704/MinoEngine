#pragma once

#include <string>
#include <any>

enum class UniformType : unsigned int
{
	UNIFORM_INT = 0x1404,
	UNIFORM_FLOAT = 0x1406,
	UNIFORM_VEC2 = 0x8B50,
	UNIFORM_VEC3 = 0x8B51,
	UNIFORM_VEC4 = 0x8B52,
	UNIFORM_MAT4 = 0x8B5C,
	UNIFORM_SAMPLER_2D = 0x8B5E,
};

struct UniformInfo
{
	UniformType type;
	std::string name;
	unsigned int location;
	std::any defaultValue;
};