#pragma once

#include <imgui.h>
#include <Color.h>
#include <Math/FVector2.h>

class Converter
{
public:
	static ImVec4 ToImVec4(const Color& p_value);
	static Color ToColor(const ImVec4& p_value);
	static ImVec2 ToImVec2(const FVector2& p_value);
	static FVector2 ToFVector2(const ImVec2& p_value);
};