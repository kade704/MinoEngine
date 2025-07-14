#pragma once

#include "AButton.h"
#include "../Color.h"
#include "../Converter.h"
#include <imgui_internal.h>

namespace Widget
{
	class Button : public AButton
	{
	public:
		Button(const std::string& p_label = "", const FVector2& p_size = FVector2(0.f, 0.f), bool p_disabled = false) :
			label(p_label),
			size(p_size),
			disabled(p_disabled)
		{
			auto& style = ImGui::GetStyle();

			idleBackgroundColor = Converter::ToColor(style.Colors[ImGuiCol_Button]);
			hoveredBackgroundColor = Converter::ToColor(style.Colors[ImGuiCol_ButtonHovered]);
			clickedBackgroundColor = Converter::ToColor(style.Colors[ImGuiCol_ButtonActive]);
			textColor = Converter::ToColor(style.Colors[ImGuiCol_Text]);
		}

	protected:
		void _Draw_Impl() override
		{
			auto& style = ImGui::GetStyle();

			auto defaultIdleColor = style.Colors[ImGuiCol_Button];
			auto defaultHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
			auto defaultClickedColor = style.Colors[ImGuiCol_ButtonActive];
			auto defaultTextColor = style.Colors[ImGuiCol_Text];

			style.Colors[ImGuiCol_Button] = Converter::ToImVec4(idleBackgroundColor);
			style.Colors[ImGuiCol_ButtonHovered] = Converter::ToImVec4(hoveredBackgroundColor);
			style.Colors[ImGuiCol_ButtonActive] = Converter::ToImVec4(clickedBackgroundColor);
			style.Colors[ImGuiCol_Text] = Converter::ToImVec4(textColor);

			if (ImGui::ButtonEx((label + m_widgetID).c_str(), Converter::ToImVec2(size), disabled ? ImGuiItemFlags_Disabled : 0))
				ClickedEvent.Invoke();

			style.Colors[ImGuiCol_Button] = defaultIdleColor;
			style.Colors[ImGuiCol_ButtonHovered] = defaultHoveredColor;
			style.Colors[ImGuiCol_ButtonActive] = defaultClickedColor;
			style.Colors[ImGuiCol_Text] = defaultTextColor;
		}

	public:
		std::string label;
		FVector2 size;
		bool disabled = false;

		Color idleBackgroundColor;
		Color hoveredBackgroundColor;
		Color clickedBackgroundColor;

		Color textColor;
	};
}