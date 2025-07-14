#pragma once

#include "ProfilerSpy.h"
#include "ProfilerReport.h"

#include <unordered_map>

namespace Analytics
{
	class Profiler
	{
	public:
		Profiler();
		~Profiler();

		void ClearHistory();
		void Update(float deltaTime);
		ProfilerReport GenerateReport();
		static void Save(ProfilerSpy& spy);


	private:
		static std::unordered_map<std::string, double> __ELPASED_HISTORY;
		static std::unordered_map<std::string, uint64_t> __CALLS_COUNTER;
		static uint32_t __ELAPSED_FRAMES;
		std::chrono::steady_clock::time_point m_lastTime;
	};

}