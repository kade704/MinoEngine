#pragma once

#include <string>

#include "Event.h"

#define MINO_LOG(message) Logger::Log(message, Logger::LogLevel::Log)
#define MINO_WARNING(message) Logger::Log(message, Logger::LogLevel::Warning)
#define MINO_ERROR(message) Logger::Log(message, Logger::LogLevel::Error)

class Logger
{
public:
	enum class LogLevel
	{
		Log,
		Warning,
		Error,
	};

	struct LogData
	{
		std::string message;
		LogLevel logLevel;
		std::string date;
	};

public:
	static void Log(const std::string& message, LogLevel logLevel);
	static Event<const LogData&> LogEvent;
};

