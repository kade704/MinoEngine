#pragma once

#include "IPlugin.h"
#include "WidgetContainer.h"

class ContextualMenu : public IPlugin, public WidgetContainer
{
public:
	void Execute()
	{
		if (ImGui::BeginPopupContextItem())
		{
			DrawWidgets();
			ImGui::EndPopup();
		}
	}
	void Close()
	{
		ImGui::CloseCurrentPopup();
	}
};