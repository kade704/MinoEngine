#pragma once

#include "Window.h"
#include "InputState.h"
#include "EKey.h"
#include "EMouseButton.h"
#include "Event.h"

#include <unordered_map>

enum class EKeyState
{
	KEY_UP = 0,
	KEY_DOWN = 1
};

enum class EMouseButtonState
{
	MOUSE_UP = 0,
	MOUSE_DOWN = 1
};

class InputManager
{
public:
	InputManager(Window& window);
	~InputManager();

	EKeyState GetKeyState(EKey p_key) const;
	EMouseButtonState GetMouseButtonState(EMouseButton p_button) const;

	bool IsKeyPressed(EKey p_key) const;
	bool IsKeyReleased(EKey p_key) const;
	bool IsMouseButtonPressed(EMouseButton p_button) const;
	bool IsMouseButtonReleased(EMouseButton p_button) const;

	std::pair<double, double> GetMousePosition() const;
	void SetMousePosition(double p_x, double p_y);

	void ClearEvents();

private:
	void OnKeyPressed(int p_key);
	void OnKeyReleased(int p_key);
	void OnMouseButtonPressed(int p_button);
	void OnMouseButtonReleased(int p_button);


private:
	Window& m_window;

	ListenerID m_keyPressedListener;
	ListenerID m_keyReleasedListener;
	ListenerID m_mouseButtonPressedListener;
	ListenerID m_mouseButtonReleasedListener;

	std::unordered_map<EKey, EKeyState>					m_keyEvents;
	std::unordered_map<EMouseButton, EMouseButtonState>	m_mouseButtonEvents;
};

