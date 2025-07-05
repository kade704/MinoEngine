#pragma once

#include "Text.h"
#include "../Converter.h"
#include <string>

namespace Widget
{
	class TextColored : public Text
	{
	public:
		TextColored(const std::string& p_content = "", const Color& p_color = Color(1.0f, 1.0f, 1.0f)) :
			Text(p_content), color(p_color)
		{
		}

	protected:
		virtual void _Draw_Impl() override
		{
			ImGui::TextColored(Converter::ToImVec4(color), content.c_str());
		}

	public:
		Color color;
	};
}