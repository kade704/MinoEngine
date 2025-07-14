#include "PanelMenubar.h"

#include <imgui.h>

void Panel::PanelMenubar::_Draw_Impl()
{
	if (!m_widgets.empty() && ImGui::BeginMainMenuBar())
	{
		DrawWidgets();
		ImGui::EndMainMenuBar();
	}
}
