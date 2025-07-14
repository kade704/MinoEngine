#pragma once

#include "../WidgetContainer.h"

namespace Widget
{
	class Group : public AWidget, public WidgetContainer
	{
	protected:
		virtual void _Draw_Impl() override
		{
			DrawWidgets();
		}
	};
}
