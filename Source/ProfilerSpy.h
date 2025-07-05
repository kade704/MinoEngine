#pragma once

#include <string>
#include <chrono>
#include <memory>

#define PROFILER_SPY(name)\
	std::unique_ptr<Analytics::ProfilerSpy> __profiler_spy__ = \
	std::make_unique<Analytics::ProfilerSpy>(name)

namespace Analytics
{
	struct ProfilerSpy final
	{
		ProfilerSpy(const std::string& name);
		~ProfilerSpy();

		std::string name;
		std::chrono::steady_clock::time_point start;
		std::chrono::steady_clock::time_point end;
	};
}