#include "Converter.h"

ImVec4 Converter::ToImVec4(const Color& p_value)
{
	return ImVec4(p_value.r, p_value.g, p_value.b, p_value.a);
}

Color Converter::ToColor(const ImVec4& p_value)
{
	return Color(p_value.x, p_value.y, p_value.z, p_value.w);
}

ImVec2 Converter::ToImVec2(const FVector2& p_value)
{
	return ImVec2(p_value.x, p_value.y);
}

FVector2 Converter::ToFVector2(const ImVec2& p_value)
{
	return FVector2(p_value.x, p_value.y);
}
