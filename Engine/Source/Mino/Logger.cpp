#include "Logger.h"
#include <iostream>
#include <Date.h>


Event<const Logger::LogData&> Logger::LogEvent;

void Logger::Log(const std::string& p_message, LogLevel p_logLevel)
{
	std::string header;
	switch (p_logLevel)
	{
	case LogLevel::Log:
		header = "[Log] ";
		break;
	case LogLevel::Warning:
		header = "[Warning] ";
		break;
	case LogLevel::Error:
		header = "[Error] ";
		break;
	}

	std::cerr << header << p_message << std::endl;

	LogData logData{ p_message, p_logLevel, Date::GetDateAsString() };
	LogEvent.Invoke(logData);
}
