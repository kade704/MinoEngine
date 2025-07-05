#include <algorithm>

#include <Panel/Console.h>
#include <Widget/Button.h>
#include <Widget/Spacing.h>
#include <Widget/Separator.h>

std::pair<Color, std::string> GetWidgetSettingsFromLogData(const Logger::LogData& p_logData)
{
	Color logColor;
	std::string logHeader;
	std::string logDateFormated = "[";
	bool isSecondPart = false;
	std::for_each(p_logData.date.begin(), p_logData.date.end(), [&logDateFormated, &isSecondPart](char c)
		{
			if (isSecondPart)
			logDateFormated.push_back(c == '-' ? ':' : c);

	if (c == '_')
		isSecondPart = true;
		});

	logDateFormated += "] ";

	switch (p_logData.logLevel)
	{
	default:
	case Logger::LogLevel::Log:		return { { 0.f, 1.f, 1.f, 1.f }, logDateFormated };
	case Logger::LogLevel::Warning:	return { { 1.f, 1.f, 0.f, 1.f }, logDateFormated };
	case Logger::LogLevel::Error:	return { { 1.f, 0.f, 0.f, 1.f }, logDateFormated };
	}
}

Panel::Console::Console(const std::string& p_title) :
	PanelWindow(p_title)
{
	auto& clearButton = CreateWidget<Widget::Button>("Clear");
	clearButton.size = { 50.f, 0.f };
	clearButton.idleBackgroundColor = { 0.5f, 0.f, 0.f };
	clearButton.ClickedEvent += std::bind(&Console::Clear, this);

	CreateWidget<Widget::Separator>();

	m_logGroup = &CreateWidget<Widget::Group>();
	m_logGroup->ReverseDrawOrder();

	Logger::LogEvent += std::bind(&Console::OnLogIntercepted, this, std::placeholders::_1);
}

void Panel::Console::OnLogIntercepted(const Logger::LogData& p_logData)
{
	auto [logColor, logDate] = GetWidgetSettingsFromLogData(p_logData);

	auto& consoleItem1 = m_logGroup->CreateWidget<Widget::TextColored>(logDate + "\t" + p_logData.message, logColor);

	m_logTextWidgets[&consoleItem1] = p_logData.logLevel;
}

void Panel::Console::Clear()
{
	m_logTextWidgets.clear();
	m_logGroup->RemoveAllWidgets();
}
