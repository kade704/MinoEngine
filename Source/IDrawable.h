#pragma once

class IDrawable
{
public:
	virtual void Draw() = 0;

protected:
	virtual ~IDrawable() = default;
};