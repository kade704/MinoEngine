#include "Clock.h"

void Clock::Update()
{
	mLastTime = mCurrentTime;
	mCurrentTime = std::chrono::steady_clock::now();
	mElapsed = mCurrentTime - mLastTime;
	mDeltaTime = mElapsed.count() > 0.1 ? 0.1 : mElapsed.count();
}

float Clock::GetDeltaTime()
{
	return mDeltaTime;
}

void Clock::Initialize()
{
	mDeltaTime = 0.0f;
	mStartTime = std::chrono::steady_clock::now();
	mCurrentTime = mStartTime;
	mLastTime = mStartTime;

	mInitialized = true;
}
