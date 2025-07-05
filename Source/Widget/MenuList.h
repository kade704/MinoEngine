#pragma once

#include "../Event.h"
#include "Group.h"

namespace Widget
{
	class MenuList : public Group
	{
	public:
		MenuList(const std::string& p_name, bool p_locked = false) :
			name(p_name), locked(p_locked)
		{
		}

	protected:
		virtual void _Draw_Impl() override
		{
			if (ImGui::BeginMenu(name.c_str(), !locked))
			{
				if (!m_opened)
				{
					ClickedEvent.Invoke();
					m_opened = true;
				}

				DrawWidgets();
				ImGui::EndMenu();
			}
			else
			{
				m_opened = false;
			}
		}

	public:
		std::string name;
		bool locked;
		Event<> ClickedEvent;

	private:
		bool m_opened;
	};
}