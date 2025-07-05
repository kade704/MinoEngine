#pragma once

#include "APanel.h"

namespace Panel
{
	class PanelMenubar : public APanel
	{
	protected:
		void _Draw_Impl() override;
	};
}