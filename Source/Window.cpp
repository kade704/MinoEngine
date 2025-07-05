#include <string>
#include <GLFW/glfw3.h>

#include "Window.h"

Window::Window(WindowSettings& windowSettings):
    mTitle(windowSettings.title),
    mWidth(windowSettings.width),
    mHeight(windowSettings.height)
{
    CreateGlfwWindow();
}

Window::~Window()
{
    glfwDestroyWindow(mGlfwWindow);
}

void Window::SetShouldClose(bool value) const
{
    glfwSetWindowShouldClose(mGlfwWindow, value);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(mGlfwWindow);
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(mGlfwWindow);
}

void Window::MakeContextCurrent() const
{
    glfwMakeContextCurrent(mGlfwWindow);
}

void Window::CreateGlfwWindow()
{
    mGlfwWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);

    if (!mGlfwWindow)
    {
        throw "Failed to create GLFW window";
    }
}

GLFWwindow* Window::GetGlfwWindow() const
{
    return mGlfwWindow;
}
