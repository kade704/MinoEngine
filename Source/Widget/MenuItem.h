#pragma once

#include "DataWidget.h"
#include "../Event.h"

namespace Widget
{
	class MenuItem : public DataWidget<bool>
	{
	public:
		MenuItem(const std::string& p_name, const std::string& p_shortcut = "", bool p_checkable = false, bool p_checked = false) :
			DataWidget(m_selected), name(p_name), shortcut(p_shortcut), checkable(p_checkable), checked(p_checked)
		{

		}

	protected:
		void _Draw_Impl() override
		{
			bool previousValue = checked;

			if (ImGui::MenuItem((name + m_widgetID).c_str(), shortcut.c_str(), checkable ? &checked : nullptr, enabled))
				ClickedEvent.Invoke();

			if (checked != previousValue)
			{
				ValueChangedEvent.Invoke(checked);
				this->NotifyChange();
			}
		}

	public:
		std::string name;
		std::string shortcut;
		bool checkable;
		bool checked;
		Event<> ClickedEvent;
		Event<bool> ValueChangedEvent;

	private:
		bool m_selected;
	};
}