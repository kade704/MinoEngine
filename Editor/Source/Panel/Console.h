#pragma once

#include <Mino/Panel/PanelWindow.h>
#include <Mino/Widget/Group.h>
#include <Mino/Widget/TextColored.h>
#include <Mino/Logger.h>

namespace Panel
{
	class Console : public Panel::PanelWindow
	{
	public:
		Console(const std::string& p_title);

		void OnLogIntercepted(const Logger::LogData& p_logData);
		void Clear();

	private:
		Widget::Group* m_logGroup;
		std::unordered_map<Widget::TextColored*, Logger::LogLevel> m_logTextWidgets;
	};
}
