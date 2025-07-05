#pragma once

#include "../WidgetContainer.h"
#include "../IDrawable.h"

namespace Panel
{
	class APanel : public IDrawable, public WidgetContainer
	{
	public:
		APanel();

		void Draw() override;

		const std::string& GetPanelID() const;

	protected:
		virtual void _Draw_Impl() = 0;

	protected:
		std::string m_panelID;

	private:
		static uint64_t __PANEL_ID_INCREMENT;
	};
}
