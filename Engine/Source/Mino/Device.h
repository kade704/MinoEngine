#pragma once

#include "DeviceSettings.h"

class Device
{
public:
	Device(DeviceSettings& deviceSettings);
	~Device();

	void PollEvents() const;

	float GetElapsedTime() const;

private:
	bool mIsAlive;
};

