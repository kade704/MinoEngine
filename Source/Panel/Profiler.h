#pragma once

#include "PanelWindow.h"
#include "../Profiler.h"
#include "../Widget/Text.h"
#include "../Widget/TextColored.h"
#include "../Widget/Column.h"
#include <string>

namespace Panel
{
	class Profiler : public PanelWindow
	{
	public:
		Profiler(const std::string& p_title, float p_frequency);

		void Update(float p_deltaTime);

	private:
		Color CalculateActionColor(double p_percentage) const;

	private:
		Analytics::Profiler m_profiler;
		float m_frequency;
		float m_timer = 0.0f;
		float m_fpsTimer = 0.0f;
		Widget::TextColored* m_fpsText;
		Widget::TextColored* m_elapsedFramesText;
		Widget::TextColored* m_elapsedTimeText;
		Widget::Column<5>* m_actionList;
	};
}
