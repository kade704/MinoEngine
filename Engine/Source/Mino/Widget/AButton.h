#pragma once

#include "AWidget.h"
#include "../Event.h"

namespace Widget
{
	class AButton : public AWidget
	{
	protected:
		void _Draw_Impl() override = 0;

	public:
		Event<> ClickedEvent;
	};
}