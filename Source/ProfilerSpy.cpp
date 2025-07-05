#include "ProfilerSpy.h"
#include "Profiler.h"

Analytics::ProfilerSpy::ProfilerSpy(const std::string& name) :
	name(name),
	start(std::chrono::steady_clock::now())
{
}

Analytics::ProfilerSpy::~ProfilerSpy()
{
	end = std::chrono::steady_clock::now();
	Profiler::Save(*this);
}
