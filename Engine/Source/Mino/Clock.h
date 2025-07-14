#pragma once

#include <chrono>

class Clock
{
public:
	void Initialize();
	void Update();
	float GetDeltaTime();	

private:
	std::chrono::steady_clock::time_point mStartTime;
	std::chrono::steady_clock::time_point mLastTime;
	std::chrono::steady_clock::time_point mCurrentTime;
	std::chrono::duration<double> mElapsed;
	bool mInitialized;
	float mDeltaTime;
};

