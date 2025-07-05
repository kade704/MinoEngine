#pragma once

#include "Panel/APanel.h"
#include "Canvas.h"

#include <vector>

class UIManager
{
public:
	UIManager(struct GLFWwindow* glfwWindow);
	~UIManager();

	void Render();

	void SetCanvas(Canvas& p_canvas);
	void RemoveCanvas();

private:
	void ApplyStyle();

private:
	Canvas* m_currentCanvas = nullptr;
};

