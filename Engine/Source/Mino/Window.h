#pragma once

#include "WindowSettings.h"
#include "Event.h"

enum class ECursorMode
{
	NORMAL		= 0x00034001,
	DISABLED	= 0x00034003,
	HIDDEN		= 0x00034002
};

struct GLFWwindow;

class Window
{
public:
	Event<int> KeyPressedEvent;
	Event<int> KeyReleasedEvent;
	Event<int> MouseButtonPressedEvent;
	Event<int> MouseButtonReleasedEvent;
	Event<uint16_t, uint16_t> ResizeEvent;

	Window(WindowSettings& windowSettings);
	~Window();

	static Window* FindInstance(GLFWwindow* p_glfwWindow);

	void SetShouldClose(bool value) const;
	bool ShouldClose() const;

	void SwapBuffers() const;

	void SetCursorMode(ECursorMode p_cursorMode);

	void MakeContextCurrent() const;

	std::pair<int16_t, int16_t> GetPosition() const;

	GLFWwindow* GetGlfwWindow() const;

private:
	void CreateGlfwWindow();

	void BindKeyCallback() const;
	void BindMouseCallback() const;
	void BindResizeCallback() const;

private:
	static std::unordered_map<GLFWwindow*, Window*> __WINDOWS_MAP;

	GLFWwindow* m_glfwWindow;
	std::string m_title;
	unsigned int m_width;
	unsigned int m_height;
	std::pair<int16_t, int16_t> m_position;
	ECursorMode m_cursorMode;
};

