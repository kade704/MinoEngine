#include "Profiler.h"

#include <iostream>
#include <iomanip>
#include <map>
#include <string>

std::unordered_map<std::string, double> Analytics::Profiler::__ELPASED_HISTORY;
std::unordered_map<std::string, uint64_t> Analytics::Profiler::__CALLS_COUNTER;
uint32_t Analytics::Profiler::__ELAPSED_FRAMES;

Analytics::Profiler::Profiler() :
	m_lastTime(std::chrono::steady_clock::now())
{
}

Analytics::Profiler::~Profiler()
{
}

void Analytics::Profiler::ClearHistory()
{
	__ELPASED_HISTORY.clear();
	__CALLS_COUNTER.clear();
	m_lastTime = std::chrono::steady_clock::now();
}

void Analytics::Profiler::Update(float deltaTime)
{
	++__ELAPSED_FRAMES;
}

ProfilerReport Analytics::Profiler::GenerateReport()
{
	ProfilerReport report;

	if (__ELAPSED_FRAMES == 0)
		return report;

	std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - m_lastTime;

	report.elapsedFrames = __ELAPSED_FRAMES;
	report.elapsedTime = elapsed.count();

	std::multimap<double, std::string, std::greater<double>> sortedHistory;

	for (auto& data : __ELPASED_HISTORY)
		sortedHistory.insert(std::pair<double, std::string>(data.second, data.first));

	for (auto& data : sortedHistory)
		report.actions.push_back({ data.second, data.first, (data.first / elapsed.count()) * 100.0, __CALLS_COUNTER[data.second] });

	return report;
}

void Analytics::Profiler::Save(ProfilerSpy& spy)
{
	if (__ELPASED_HISTORY.find(spy.name) != __ELPASED_HISTORY.end())
	{
		__ELPASED_HISTORY[spy.name] += std::chrono::duration<float>(spy.end - spy.start).count();
	}
	else
	{
		__ELPASED_HISTORY[spy.name] = std::chrono::duration<float>(spy.end - spy.start).count();
	}

	if (__CALLS_COUNTER.find(spy.name) != __CALLS_COUNTER.end())
	{
		__CALLS_COUNTER[spy.name]++;
	}
	else
	{
		__CALLS_COUNTER[spy.name] = 1;
	}
}

