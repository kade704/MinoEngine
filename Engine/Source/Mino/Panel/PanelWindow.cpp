#include "PanelWindow.h"

#include <imgui.h>

Panel::PanelWindow::PanelWindow(const std::string& p_name) :
	name(p_name)
{
}

bool Panel::PanelWindow::IsHovered() const
{
	return m_hovered;
}

bool Panel::PanelWindow::IsFocused() const
{
	return m_focused;
}

void Panel::PanelWindow::_Draw_Impl()
{
	if (ImGui::Begin((name + m_panelID).c_str()))
	{
		m_hovered = ImGui::IsWindowHovered();
		m_focused = ImGui::IsWindowFocused();

		Update();
		DrawWidgets();
	}

	ImGui::End();
}
