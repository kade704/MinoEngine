#pragma once

#include <Mino/Actor.h>

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

	bool IsSnappedBehaviourEnabled() const;

	void StartPicking(Actor& p_target, const FVector3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction);
	void StopPicking();

	void SetCurrentMouse(const FVector2& p_mousePosition);

	FVector2 GetScreenDirection(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize) const;
	FVector3 GetRealDirection(bool p_relative = false) const;

	bool IsPicking() const;

	EDirection GetDirection() const;
	FVector3 GetFakeDirection() const;

	void ApplyTranslation(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize) const;
	void ApplyRotation(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize) const;
	void ApplyScale(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize) const;
	void ApplyOperation(const FMatrix4& p_viewMatrix, const FMatrix4& p_projectionMatrix, const FVector2& p_viewSize);


private:
	bool m_firstMouse = true;
	float m_distanceToActor = 0.0f;
	Actor* m_target = nullptr;
	EGizmoOperation m_currentOperation;
	EDirection m_direction;
	FTransform m_originalTransform;
	FVector2 m_originMouse;
	FVector2 m_currentMouse;
	FVector2 m_screenDirection;
};

