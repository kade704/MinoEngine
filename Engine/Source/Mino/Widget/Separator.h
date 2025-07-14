#pragma once

#include "AWidget.h"

namespace Widget
{
	class Separator : public AWidget
	{
	protected:
		void _Draw_Impl() override
		{
			ImGui::Separator();
		}
	};
}