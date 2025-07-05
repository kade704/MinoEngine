#pragma once

#include "InputManager.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "Panel/AView.h"
#include "Window.h"

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

private:
	void HandleCameraFPSMouse(const FVector2& p_mouseOffset, bool p_firstMouse);
	void HandleCameraFPSKeyboard(float deltaTime);
	FVector3 RemoveRoll(const FVector3& p_ypr);

private:
	InputManager& m_inputManager;
	Window& m_window;
	Panel::AView& m_view;
	FVector3& m_cameraPosition;
	FQuaternion& m_cameraRotation;
	float m_cameraMoveSpeed = 15.0f;
	float m_mouseSensitivity = 0.12f;
	FVector3 m_targetSpeed;
	FVector3 m_currentMovementSpeed;
	double m_lastMousePosX = 0.0;
	double m_lastMousePosY = 0.0;
	bool m_firstMouse = true;
	FVector3 m_ypr;
};