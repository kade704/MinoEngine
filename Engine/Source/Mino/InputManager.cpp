#include <GLFW/glfw3.h>

#include "InputManager.h"

InputManager::InputManager(Window& window) :
    m_window(window)
{
}

InputManager::~InputManager()
{
}

EKeyState InputManager::GetKeyState(EKey p_key) const
{
    switch (glfwGetKey(m_window.GetGlfwWindow(), static_cast<int>(p_key)))
    {
    case GLFW_PRESS:	return EKeyState::KEY_DOWN;
    case GLFW_RELEASE:	return EKeyState::KEY_UP;
    }

    return EKeyState::KEY_UP;
}

EMouseButtonState InputManager::GetMouseButtonState(EMouseButton p_button) const
{
    switch (glfwGetMouseButton(m_window.GetGlfwWindow(), static_cast<int>(p_button)))
    {
    case GLFW_PRESS:	return EMouseButtonState::MOUSE_DOWN;
    case GLFW_RELEASE:	return EMouseButtonState::MOUSE_UP;
    }

    return EMouseButtonState::MOUSE_UP;
}

bool InputManager::IsKeyPressed(EKey p_key) const
{
    return m_keyEvents.find(p_key) != m_keyEvents.end() && m_keyEvents.at(p_key) == EKeyState::KEY_DOWN;
}

bool InputManager::IsKeyReleased(EKey p_key) const
{
    return m_keyEvents.find(p_key) != m_keyEvents.end() && m_keyEvents.at(p_key) == EKeyState::KEY_UP;
}

bool InputManager::IsMouseButtonPressed(EMouseButton p_button) const
{
    return m_mouseButtonEvents.find(p_button) != m_mouseButtonEvents.end() && m_mouseButtonEvents.at(p_button) == EMouseButtonState::MOUSE_DOWN;
}

bool InputManager::IsMouseButtonReleased(EMouseButton p_button) const
{
    return m_mouseButtonEvents.find(p_button) != m_mouseButtonEvents.end() && m_mouseButtonEvents.at(p_button) == EMouseButtonState::MOUSE_UP;
}

std::pair<double, double> InputManager::GetMousePosition() const
{
    std::pair<double, double> result;
    glfwGetCursorPos(m_window.GetGlfwWindow(), &result.first, &result.second);
    return result;;
}

void InputManager::ClearEvents()
{
    m_keyEvents.clear();
    m_mouseButtonEvents.clear();
}

void InputManager::OnKeyPressed(int p_key)
{
    m_keyEvents[static_cast<EKey>(p_key)] = EKeyState::KEY_DOWN;
}

void InputManager::OnKeyReleased(int p_key)
{
    m_keyEvents[static_cast<EKey>(p_key)] = EKeyState::KEY_UP;
}

void InputManager::OnMouseButtonPressed(int p_button)
{
    m_mouseButtonEvents[static_cast<EMouseButton>(p_button)] = EMouseButtonState::MOUSE_DOWN;
}

void InputManager::OnMouseButtonReleased(int p_button)
{
    m_mouseButtonEvents[static_cast<EMouseButton>(p_button)] = EMouseButtonState::MOUSE_UP;
}