#include "CameraController.h"
#include "Context.h"
#include "EditorAction.h"

CameraController::CameraController
(
    Panel::AView& p_view,
    Camera& p_camera,
    FVector3& p_position,
    FQuaternion& p_rotation
) :
    m_inputManager(*EDITOR_CONTEXT(inputManager)),
    m_window(*EDITOR_CONTEXT(window)),
    m_view(p_view),
    m_camera(p_camera),
    m_cameraPosition(p_position),
    m_cameraRotation(p_rotation)
{
    m_camera.SetFov(60.0f);
}

void CameraController::HandleInputs(float deltaTime)
{
    HandleMouseReleased();

    if (m_view.IsHovered())
	{
		HandleMousePressed();
    }

    if (m_rightMousePressed || m_middleMousePressed || m_leftMousePressed)
	{
        auto [xPos, yPos] = m_inputManager.GetMousePosition();
        bool wasFirstMouse = m_firstMouse;

        if (m_firstMouse)
        {
            m_lastMousePosX = xPos;
            m_lastMousePosY = yPos;
            m_firstMouse = false;
        }

        FVector2 mouseOffset
        {
            static_cast<float>(xPos - m_lastMousePosX),
            static_cast<float>(m_lastMousePosY - yPos)
        };

        m_lastMousePosX = xPos;
        m_lastMousePosY = yPos;

        if (m_rightMousePressed)
        {
            HandleCameraFPSMouse(mouseOffset, wasFirstMouse);
        }
    }

    HandleCameraFPSKeyboard(deltaTime);
}

const FVector3& CameraController::GetPosition() const
{
    return m_cameraPosition;
}

const FQuaternion& CameraController::GetRotation() const
{
    return m_cameraRotation;
}

bool CameraController::IsRightMousePressed() const
{
    return m_rightMousePressed;
}

FVector3 RemoveRoll(const FVector3& p_ypr)
{
    FVector3 result = p_ypr;

    if (result.z >= 179.0f || result.z <= -179.0f)
    {
        result.x += result.z;
        result.y = 180.0f - result.y;
        result.z = 0.0f;
    }

    if (result.x > 180.0f) result.x -= 360.0f;
    if (result.x < -180.0f) result.x += 360.0f;

    return result;
}


void CameraController::HandleCameraFPSMouse(const FVector2& p_mouseOffset, bool p_firstMouse)
{
    auto mouseOffset = p_mouseOffset * m_mouseSensitivity;

    if (p_firstMouse)
    {
        m_ypr = FQuaternion::EulerAngles(m_cameraRotation);
        m_ypr = RemoveRoll(m_ypr);
    }

    m_ypr.y -= mouseOffset.x;
    m_ypr.x += -mouseOffset.y;
    m_ypr.x = std::max(std::min(m_ypr.x, 90.0f), -90.0f);

    m_cameraRotation = FQuaternion(m_ypr);
}

void CameraController::HandleCameraFPSKeyboard(float p_deltaTime)
{
    m_targetSpeed = FVector3(0.f, 0.f, 0.f);

    if (m_rightMousePressed)
    {
        bool run = m_inputManager.GetKeyState(EKey::KEY_LEFT_SHIFT) == EKeyState::KEY_DOWN;
        float velocity = m_cameraMoveSpeed * p_deltaTime * (run ? 2.0f : 1.0f);

        if (m_inputManager.GetKeyState(EKey::KEY_W) == EKeyState::KEY_DOWN)
            m_targetSpeed += m_cameraRotation * FVector3::Forward * velocity;
        if (m_inputManager.GetKeyState(EKey::KEY_S) == EKeyState::KEY_DOWN)
            m_targetSpeed -= m_cameraRotation * FVector3::Forward * velocity;
        if (m_inputManager.GetKeyState(EKey::KEY_A) == EKeyState::KEY_DOWN)
            m_targetSpeed += m_cameraRotation * FVector3::Right * velocity;
        if (m_inputManager.GetKeyState(EKey::KEY_D) == EKeyState::KEY_DOWN)
            m_targetSpeed -= m_cameraRotation * FVector3::Right * velocity;
    }

    m_currentMovementSpeed = FVector3::Lerp(m_currentMovementSpeed, m_targetSpeed, 10.0f * p_deltaTime);
    m_cameraPosition += m_currentMovementSpeed;
}

void CameraController::HandleMousePressed()
{
    if (m_inputManager.IsMouseButtonPressed(EMouseButton::MOUSE_BUTTON_LEFT))
	{
		m_leftMousePressed = true;
	}

	if (m_inputManager.IsMouseButtonPressed(EMouseButton::MOUSE_BUTTON_MIDDLE))
	{
		m_middleMousePressed = true;
	}
	if (m_inputManager.IsMouseButtonPressed(EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_rightMousePressed = true;
		m_window.SetCursorMode(ECursorMode::DISABLED);
	}
}

void CameraController::HandleMouseReleased()
{
    if (m_leftMousePressed && m_inputManager.IsMouseButtonReleased(EMouseButton::MOUSE_BUTTON_LEFT))
    {
        m_leftMousePressed = false;
        m_firstMouse = true;
    }

    if (m_middleMousePressed && m_inputManager.IsMouseButtonReleased(EMouseButton::MOUSE_BUTTON_MIDDLE))
    {
        m_middleMousePressed = false;
        m_firstMouse = true;
    }
    if (m_rightMousePressed && m_inputManager.IsMouseButtonReleased(EMouseButton::MOUSE_BUTTON_RIGHT))
    {
        m_rightMousePressed = false;
        m_firstMouse = true;
        m_window.SetCursorMode(ECursorMode::NORMAL);
    }
}
