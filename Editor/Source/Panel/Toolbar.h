#pragma once

#include <Mino/Panel/PanelWindow.h>
#include <Mino/Widget/ButtonImage.h>

namespace Panel
{
	class Toolbar : public PanelWindow
	{
	public:
		Toolbar(const std::string& p_title);

		void _Draw_Impl() override;

	private:
		Widget::ButtonImage* m_playButton;
		Widget::ButtonImage* m_pauseButton;
		Widget::ButtonImage* m_stopButton;
		Widget::ButtonImage* m_nextButton;
	};
}
