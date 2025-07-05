#pragma once

#include <Widget/AWidget.h>

namespace Widget
{
	class Spacing : public AWidget
	{
	public:
		Spacing(uint16_t p_spaces = 1) :
			spaces(p_spaces)
		{
		}

	protected:
		void _Draw_Impl() override
		{
			for (uint16_t i = 0; i < spaces; ++i)
			{
				ImGui::Spacing();

				if (i + 1 < spaces)
					ImGui::SameLine();
			}
		}

	public:
		uint16_t spaces = 1;
	};
}
