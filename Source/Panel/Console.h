#pragma once

#include <Panel/PanelWindow.h>
#include <Widget/Group.h>
#include <Widget/TextColored.h>
#include <Logger.h>

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
