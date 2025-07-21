#pragma once

#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "Math/FMatrix4.h"
#include "Color.h"

class Camera
{
public:
	Camera();
	~Camera();

	void CacheMatrices(uint16_t windowWidth, uint16_t windowHeight, const FVector3& position, const FQuaternion& rotation);
	void CacheProjectionMatrix(uint16_t windowWidth, uint16_t windowHeight);
	void CacheViewMatrix(const FVector3& position, const FQuaternion& rotation);
	
	const FMatrix4& GetProjectionMatrix() const;
	const FMatrix4& GetViewMatrix() const;
	const FVector3& GetClearColor() const;
	float GetFov() const;
	float GetNear() const;
	float GetFar() const;

	void SetFov(float value);
	void SetNear(float value);
	void SetFar(float value);
	void SetClearColor(const FVector3& color);

	FMatrix4 CalculateProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight) const;
	FMatrix4 CalculateViewMatrix(const FVector3& p_position, const FQuaternion& p_rotation) const;

private:
	FMatrix4 m_projectionMatrix;
	FMatrix4 m_viewMatrix;
	float m_fov;
	float m_near;
	float m_far;
	FVector3 m_clearColor;
};

