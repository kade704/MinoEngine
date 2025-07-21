#pragma once

#include "Panel/AView.h"

#include <Mino/InputManager.h>
#include <Mino/Math/FVector3.h>
#include <Mino/Math/FQuaternion.h>
#include <Mino/Window.h>

class EditorPanel;

class CameraController
{
public:
	CameraController
	(
		Panel::AView& p_view,
		Camera& p_camera,
		FVector3& p_position,
		FQuaternion& p_rotation
	);

	void HandleInputs(float deltaTime);

	const FVector3& GetPosition() const;
	const FQuaternion& GetRotation() const;

	bool IsRightMousePressed() const;

private:
	void HandleCameraFPSMouse(const FVector2& p_mouseOffset, bool p_firstMouse);
	void HandleCameraFPSKeyboard(float deltaTime);
	void HandleMousePressed();
	void HandleMouseReleased();

private:
	InputManager& m_inputManager;
	Window& m_window;
	Panel::AView& m_view;
	Camera& m_camera;
	FVector3& m_cameraPosition;
	FQuaternion& m_cameraRotation;

	bool m_leftMousePressed = false;
	bool m_middleMousePressed = false;
	bool m_rightMousePressed = false;
	
	FVector3 m_targetSpeed;
	FVector3 m_currentMovementSpeed;
	bool m_firstMouse = true;
	double m_lastMousePosX = 0.0;
	double m_lastMousePosY = 0.0;
	FVector3 m_ypr;
	float m_mouseSensitivity = 0.12f;
	float m_cameraMoveSpeed = 15.0f;
};