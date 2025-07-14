#include "Profiler.h"
#include "../Context.h"
#include "../imgui/imgui.h"

Panel::Profiler::Profiler(const std::string& p_title, float p_frequency) :
	PanelWindow(p_title),
	m_frequency(p_frequency)
{
	CreateWidget<Widget::Text>("Profiler state : ");
	m_fpsText = &CreateWidget<Widget::TextColored>("");
	m_elapsedFramesText = &CreateWidget<Widget::TextColored>("");
	m_elapsedTimeText = &CreateWidget<Widget::TextColored>("");
	m_actionList = &CreateWidget<Widget::Column<5>>();
	m_actionList->widths = { 300.f, 100.f, 100.f, 100.f, 200.f };
}

void Panel::Profiler::Update(float p_deltaTime)
{
	m_timer += p_deltaTime;
	m_fpsTimer += p_deltaTime;

	while (m_fpsTimer >= 0.07f)
	{
		m_fpsText->content = "FPS: " + std::to_string(static_cast<int>(1.0f / p_deltaTime));
		m_fpsTimer -= 0.07f;
	}

	m_profiler.Update(p_deltaTime);

	while (m_timer >= m_frequency)
	{
		ProfilerReport report = m_profiler.GenerateReport();
		m_profiler.ClearHistory();
		m_actionList->RemoveAllWidgets();

		m_elapsedFramesText->content = "Elapsed frames: " + std::to_string(report.elapsedFrames);
		m_elapsedTimeText->content = "Elapsed time: " + std::to_string(report.elapsedTime);

		m_actionList->CreateWidget<Widget::Text>("Action");
		m_actionList->CreateWidget<Widget::Text>("Total duration");
		m_actionList->CreateWidget<Widget::Text>("Frame duration");
		m_actionList->CreateWidget<Widget::Text>("Frame load");
		m_actionList->CreateWidget<Widget::Text>("Total calls");

		for (auto& action : report.actions)
		{
			auto color = CalculateActionColor(action.percentage);
			m_actionList->CreateWidget<Widget::TextColored>(action.name, color);
			m_actionList->CreateWidget<Widget::TextColored>(std::to_string(action.duration) + "s", color);
			m_actionList->CreateWidget<Widget::TextColored>(std::to_string(action.duration / action.calls) + "s", color);
			m_actionList->CreateWidget<Widget::TextColored>(std::to_string(action.percentage) + "%%", color);
			m_actionList->CreateWidget<Widget::TextColored>(std::to_string(action.calls) + " calls", color);
		}

		m_timer -= m_frequency;
	}
}

Color Panel::Profiler::CalculateActionColor(double p_percentage) const
{
	if (p_percentage <= 25.0f)		return Color(0.0f, 1.0f, 0.0f);
	else if (p_percentage <= 50.0f) return Color(1.0f, 1.0f, 0.0f);
	else if (p_percentage <= 75.0f) return Color(1.0f, 0.6f, 0.0f);
	else							return Color(1.0f, 0.0f, 0.0f);
}
