#pragma once

#include "Context.h"

enum class EGizmoOperation;

class EditorRenderer
{
public:
	EditorRenderer(Context& p_context);

	void RenderModelOutline(const FMatrix4& p_worldMatrix, Model& p_model, float p_width);
	void RenderActorOutlinePass(Actor& p_actor, bool p_isSelected);
	void RenderGizmo(const FVector3& p_position, const FQuaternion& p_rotation, EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis = -1);
	void RenderSceneForActorPicking();
	void PreparePickingMaterial(Actor& p_actor, Material& p_material);
	void UpdateLights(Scene& p_scene);
	void RenderLights();

private:
	Context& m_context;
	Material m_outlineMaterial;
	Material m_gizmoArrowMaterial;
	Material m_gizmoPickingMaterial;
	Material m_actorPickingMaterial;
	Material m_lightMaterial;
};

