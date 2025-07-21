#include <string>
#include <GLFW/glfw3.h>

#include "Window.h"

std::unordered_map<GLFWwindow*, Window*> Window::__WINDOWS_MAP;

Window::Window(WindowSettings& windowSettings):
    m_title(windowSettings.title),
    m_width(windowSettings.width),
    m_height(windowSettings.height)
{
    CreateGlfwWindow();

    SetCursorMode(ECursorMode::NORMAL);

    BindKeyCallback();
    BindMouseCallback();
    BindResizeCallback();
}

Window::~Window()
{
    glfwDestroyWindow(m_glfwWindow);
}

Window *Window::FindInstance(GLFWwindow *p_glfwWindow)
{
    return __WINDOWS_MAP.find(p_glfwWindow) != __WINDOWS_MAP.end() ? __WINDOWS_MAP[p_glfwWindow] : nullptr;
}

void Window::SetShouldClose(bool value) const
{
    glfwSetWindowShouldClose(m_glfwWindow, value);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_glfwWindow);
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(m_glfwWindow);
}

void Window::SetCursorMode(ECursorMode p_cursorMode)
{
    m_cursorMode = p_cursorMode;
    glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, static_cast<int>(m_cursorMode));
}

void Window::MakeContextCurrent() const
{
    glfwMakeContextCurrent(m_glfwWindow);
}

void Window::CreateGlfwWindow()
{
    m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

    if (!m_glfwWindow)
    {
        throw "Failed to create GLFW window";
    }
    else
    {
        auto[x, y] = GetPosition();
		m_position.first = x;
		m_position.second = y;

        __WINDOWS_MAP[m_glfwWindow] = this;
    }
}

void Window::BindKeyCallback() const
{
    auto keyCallback = [](GLFWwindow* p_window, int p_key, int p_scancode, int p_action, int p_mods)
	{
		Window* windowInstance = FindInstance(p_window);

		if (windowInstance)
		{
			if (p_action == GLFW_PRESS)
				windowInstance->KeyPressedEvent.Invoke(p_key);

			if (p_action == GLFW_RELEASE)
				windowInstance->KeyReleasedEvent.Invoke(p_key);
		}
	};

	glfwSetKeyCallback(m_glfwWindow, keyCallback);
}

void Window::BindMouseCallback() const
{
    auto mouseButtonCallback = [](GLFWwindow* p_window, int p_button, int p_action, int p_mods)
    {
        Window* windowInstance = FindInstance(p_window);

        if (windowInstance)
        {
            if (p_action == GLFW_PRESS)
                windowInstance->MouseButtonPressedEvent.Invoke(p_button);

            if (p_action == GLFW_RELEASE)
                windowInstance->MouseButtonReleasedEvent.Invoke(p_button);
        }
    };

    glfwSetMouseButtonCallback(m_glfwWindow, mouseButtonCallback);
}

void Window::BindResizeCallback() const
{
    auto resizeCallback = [](GLFWwindow* p_window, int p_width, int p_height)
	{
		Window* windowInstance = FindInstance(p_window);

		if (windowInstance)
		{
			windowInstance->ResizeEvent.Invoke(static_cast<uint16_t>(p_width), static_cast<uint16_t>(p_height));
		}
	};

	glfwSetWindowSizeCallback(m_glfwWindow, resizeCallback);
}

GLFWwindow* Window::GetGlfwWindow() const
{
    return m_glfwWindow;
}

std::pair<int16_t, int16_t> Window::GetPosition() const
{
    int x, y;
	glfwGetWindowPos(m_glfwWindow, &x, &y);
	return std::make_pair(static_cast<int16_t>(x), static_cast<int16_t>(y));
}
