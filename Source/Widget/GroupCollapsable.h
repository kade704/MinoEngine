#pragma once

#include "Group.h"
#include "../Event.h"

namespace Widget
{
	class GroupCollapsable : public Group
	{
	public:
		GroupCollapsable(const std::string& p_name = "") :
			name(p_name)
		{
		}

	protected:
		virtual void _Draw_Impl() override
		{
			bool previouslyOpened = opened;

			if (ImGui::CollapsingHeader(name.c_str(), closable ? &opened : nullptr))
				Group::_Draw_Impl();

			if (opened != previouslyOpened)
			{
				if (opened)
					OpenEvent.Invoke();
				else
					CloseEvent.Invoke();
			}
		}

	public:
		std::string name;
		bool closable = false;
		bool opened = true;
		Event<> CloseEvent;
		Event<> OpenEvent;
	};
}