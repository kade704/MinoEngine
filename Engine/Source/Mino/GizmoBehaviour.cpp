#include "GizmoBehaviour.h"

void GizmoBehaviour::StartPicking(Actor& p_target, const FVector3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction)
{
	m_target = &p_target;
	m_firstMouse = true;
	m_originalTransform = p_target.transform.GetFTransform();
	m_distanceToActor = FVector3::Distance(p_cameraPosition, m_target->transform.GetWorldPosition());
	m_currentOperation = p_operation;
	m_direction = p_direction;
}

void GizmoBehaviour::StopPicking()
{
	m_target = nullptr;
}

void GizmoBehaviour::SetCurrentMouse(const FVector2& p_mousePosition)
{
	if (m_firstMouse)
	{
		m_currentMouse = m_originMouse = p_mousePosition;
		m_firstMouse = false;
	}
	else
	{
		m_currentMouse = p_mousePosition;
	}
}

void GizmoBehaviour::ApplyOperation(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize)
{
	switch (m_currentOperation)
	{
	case EGizmoOperation::TRANSLATE:
		ApplyTranslation(p_viewMatrix, p_projectionMatrix, p_viewSize);
		break;
	}
}

void GizmoBehaviour::ApplyTranslation(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize) const
{
	auto unitsPerPixel = 0.001f * m_distanceToActor;
	auto originPosition = m_originalTransform.GetLocalPosition();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto translationCoefficient = FVector2::Dot(totalDisplacement, screenDirection) * unitsPerPixel;

	m_target->transform.SetLocalPosition(originPosition + GetRealDirection() * translationCoefficient);
}

FVector2 GizmoBehaviour::GetScreenDirection(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize) const
{
	auto start = m_originalTransform.GetWorldPosition();
	auto end = m_originalTransform.GetWorldPosition() + GetRealDirection(true) * 0.01f;

	auto start2D = FVector2();
	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * FVector4{ start.x, start.y, start.z, 1.0f });
		auto ndcSpacePos = FVector3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((FVector2{ ndcSpacePos.x, ndcSpacePos.y } + 1.0f) / 2.0f);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		start2D = windowSpacePos;
	}

	auto end2D = FVector2();
	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * FVector4{ end.x, end.y, end.z, 1.0f });
		auto ndcSpacePos = FVector3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((FVector2{ ndcSpacePos.x, ndcSpacePos.y } + 1.0f) / 2.0f);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		end2D = windowSpacePos;
	}

	auto result = end2D - start2D;

	result.y *= -1; // Screen coordinates are reversed, so we inverse the Y

	return FVector2::Normalize(result);
}

FVector3 GizmoBehaviour::GetRealDirection(bool p_relative) const
{
	auto result = FVector3();

	switch (m_direction)
	{
	case GizmoBehaviour::EDirection::X:
		result = p_relative ? m_originalTransform.GetWorldRight() : m_originalTransform.GetLocalRight();
		break;
	case GizmoBehaviour::EDirection::Y:
		result = p_relative ? m_originalTransform.GetWorldUp() : m_originalTransform.GetLocalUp();
		break;
	case GizmoBehaviour::EDirection::Z:
		result = p_relative ? m_originalTransform.GetWorldForward() : m_originalTransform.GetLocalForward();
		break;
	}

	return result;
}

bool GizmoBehaviour::IsPicking() const
{
	return m_target;
}

GizmoBehaviour::EDirection GizmoBehaviour::GetDirection() const
{
    return m_direction;
}
