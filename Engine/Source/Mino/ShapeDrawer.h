#pragma once

#include "Renderer.h"

class ShapeDrawer
{
public:
    ShapeDrawer(Renderer& p_renderer);
    ~ShapeDrawer();

    void SetViewProjection(const FMatrix4& p_viewProjection);
    void DrawLine(const FVector3& p_start, const FVector3& p_end, const FVector3& p_color, float p_lineWidth = 1.0f);

private:
	Shader* m_lineShader = nullptr;
    Mesh* m_lineMesh = nullptr;
    Renderer& m_renderer;
};