#pragma once

#include "DataWidget.h"
#include "../Color.h"
#include "../Event.h"

namespace Widget
{
	class ColorEdit : public DataWidget<Color>
	{
	public:
		ColorEdit(bool p_enableAlpha = false, const Color& p_defaultColor = {}) :
			DataWidget<Color>(color),
			enableAlpha(p_enableAlpha),
			color(p_defaultColor)
		{

		}

	protected:
		void _Draw_Impl() override
		{
			int flags = !enableAlpha ? ImGuiColorEditFlags_NoAlpha : 0;
			bool valueChanged = false;

			if (enableAlpha)
				valueChanged = ImGui::ColorEdit4(m_widgetID.c_str(), &color.r, flags);
			else
				valueChanged = ImGui::ColorEdit3(m_widgetID.c_str(), &color.r, flags);

			if (valueChanged)
			{
				ColorChangedEvent.Invoke(color);
				this->NotifyChange();
			}
		}

	public:
		bool enableAlpha;
		Color color;
		Event<Color&> ColorChangedEvent;
	};
}