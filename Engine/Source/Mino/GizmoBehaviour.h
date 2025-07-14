#pragma once

#include "Actor.h"

enum class EGizmoOperation
{
	TRANSLATE,
	ROTATE,
	SCALE
};

class GizmoBehaviour
{
public:
	enum class EDirection
	{
		X,
		Y,
		Z
	};

	void StartPicking(Actor& p_target, const FVector3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction);
	void StopPicking();

	void SetCurrentMouse(const FVector2& p_mousePosition);
	void ApplyOperation(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize);
	void ApplyTranslation(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize) const;

	FVector2 GetScreenDirection(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize) const;
	FVector3 GetRealDirection(bool p_relative = false) const;

	bool IsPicking() const;

	EDirection GetDirection() const;

private:
	Actor* m_target = nullptr;
	EGizmoOperation m_currentOperation;
	EDirection m_direction;
	bool m_firstMouse;
	FTransform m_originalTransform;
	float m_distanceToActor;
	FVector2 m_originMouse;
	FVector2 m_currentMouse;
};

