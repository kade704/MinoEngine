#pragma once

#include "Math/FTransform.h"

struct Light
{
	enum class Type { POINT, DIRECTIONAL, SPOT, AMBIENT_BOX, AMBIENT_SPHERE };

	Light(FTransform& p_tranform, Type p_type);

	FMatrix4 GenerateMatrix() const;

	const FTransform& GetTransform() const;

	FVector3 color = { 1.f, 1.f, 1.f };
	float intensity = 1.f;
	float constant = 0.0f;
	float linear = 0.0f;
	float quadratic = 1.0f;
	float cutoff = 12.f;
	float outerCutoff = 15.f;
	float type = 0.0f;

protected:
	FTransform& m_transform;
};

