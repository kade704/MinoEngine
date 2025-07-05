#pragma once

#include <string>

#include "DataWidget.h"

namespace Widget
{
	class InputText : public DataWidget<std::string>
	{
	public:
		InputText(const std::string& p_content = "", const std::string& p_label = "") :
			DataWidget<std::string>(content), content(p_content), label(p_label)
		{
		}

	protected:
		void _Draw_Impl() override
		{
			std::string previousContent = content;

			content.resize(256, '\0');
			bool enterPressed = ImGui::InputText((label + m_widgetID).c_str(), &content[0], 256, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
			content = content.c_str();

			if (content != previousContent)
			{
				ContentChangedEvent.Invoke(content);
				this->NotifyChange();
			}

			if (enterPressed)
				EnterPressedEvent.Invoke(content);
		}

	public:
		std::string content;
		std::string label;

		Event<std::string> ContentChangedEvent;
		Event<std::string> EnterPressedEvent;
	};
}