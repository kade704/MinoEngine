#pragma once

#include <string>
#include <vector>

struct ProfilerReport
{
	struct Action
	{
		std::string name;
		double duration;
		double percentage;
		uint64_t calls;
	};

	double elapsedTime = 0.0;
	uint16_t workingThreads = 0u;
	uint32_t elapsedFrames = 0u;
	std::vector<Action> actions;
};