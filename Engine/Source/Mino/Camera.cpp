#include "Camera.h"

Camera::Camera() :
	m_fov(45.0f),
	m_near(0.1f),
	m_far(100.0f),
	m_clearColor(0.f, 0.f, 0.f)
{
}

Camera::~Camera()
{
}

void Camera::CacheMatrices(uint16_t windowWidth, uint16_t windowHeight, const FVector3& position, const FQuaternion& rotation)
{
	CacheProjectionMatrix(windowWidth, windowHeight);
	CacheViewMatrix(position, rotation);
}

void Camera::CacheProjectionMatrix(uint16_t windowWidth, uint16_t windowHeight)
{
	m_projectionMatrix = CalculateProjectionMatrix(windowWidth, windowHeight);
}

void Camera::CacheViewMatrix(const FVector3& position, const FQuaternion& rotation)
{
	m_viewMatrix = CalculateViewMatrix(position, rotation);
}

float Camera::GetFov() const
{
	return m_fov;
}

float Camera::GetNear() const
{
	return m_near;
}

float Camera::GetFar() const
{
	return m_far;
}

const FMatrix4& Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

const FMatrix4& Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

const FVector3& Camera::GetClearColor() const
{
	return m_clearColor;
}

void Camera::SetFov(float value)
{
	m_fov = value;
}

void Camera::SetNear(float value)
{
	m_near = value;
}

void Camera::SetFar(float value)
{
	m_far = value;
}

void Camera::SetClearColor(const FVector3 &color)
{
	m_clearColor = color;
}

FMatrix4 Camera::CalculateProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight) const
{
	const auto ratio = p_windowWidth / static_cast<float>(p_windowHeight);
	return FMatrix4::CreatePerspective(m_fov, ratio, m_near, m_far);
}

FMatrix4 Camera::CalculateViewMatrix(const FVector3& p_position, const FQuaternion& p_rotation) const
{
	const auto& up = p_rotation * FVector3::Up;
	const auto& forward = p_rotation * FVector3::Forward;

	return FMatrix4::CreateView
	(
		p_position.x, p_position.y, p_position.z,												// Position
		p_position.x + forward.x, p_position.y + forward.y, p_position.z + forward.z,			// LookAt (Position + Forward)
		up.x, up.y, up.z																		// Up Vector
	);
}

