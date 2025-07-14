#pragma once

#include "DataWidget.h"

#include <imgui.h>

namespace Widget
{
	class Text : public DataWidget<std::string>
	{
	public:
		Text(const std::string& p_content = "") :
			DataWidget(content),
			content(p_content)
		{
		}

	protected:
		virtual void _Draw_Impl() override
		{
			ImGui::Text(content.c_str());
		}

	public:
		std::string content;
	};
}
