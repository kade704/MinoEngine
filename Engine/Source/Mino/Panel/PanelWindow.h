#pragma once

#include <vector>
#include <string>

#include "APanelTransformable.h"

namespace Panel
{
	class PanelWindow : public APanelTransformable
	{
	public:
		PanelWindow(const std::string& p_name = "");

		bool IsHovered() const;
		bool IsFocused() const;

	protected:
		void _Draw_Impl() override;

	public:
		std::string name;

	private:
		bool m_hovered;
		bool m_focused;
	};
}
