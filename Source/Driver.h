#pragma once
class Driver
{
public:
	Driver();
	~Driver();

	bool IsActive() const;

private:
	bool mIsActive;
};

