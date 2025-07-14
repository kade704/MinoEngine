#pragma once

#include "WindowSettings.h"

struct GLFWwindow;

class Window
{
public:
	Window(WindowSettings& windowSettings);
	~Window();

	void SetShouldClose(bool value) const;
	bool ShouldClose() const;

	void SwapBuffers() const;

	void MakeContextCurrent() const;

	void CreateGlfwWindow();

	GLFWwindow* GetGlfwWindow() const;

private:
	GLFWwindow* mGlfwWindow;
	std::string mTitle;
	unsigned int mWidth;
	unsigned int mHeight;
};

