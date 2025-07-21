#pragma once

#include "Context.h"

enum class EGizmoOperation;

class EditorRenderer
{
public:
	EditorRenderer(Context& p_context);

	void InitMaterials();

	void RenderModelOutline(const FMatrix4& p_worldMatrix, Model& p_model, float p_width);
	void RenderActorOutlinePass(Actor& p_actor, bool p_isSelected);
	void RenderGizmo(const FVector3& p_position, const FQuaternion& p_rotation, EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis = -1);
	void RenderSceneForActorPicking();
	void RenderScene(const FVector3& p_cameraPosition, const Camera& p_camera);
	void RenderCameraFrustum(Component::CCamera& p_camera);
	void PreparePickingMaterial(Actor& p_actor, Material& p_material);
	FMatrix4 CalculateCameraModelMatrix(Actor& p_actor);
	void RenderGrid(const FVector3& p_viewPos, const FVector3& p_color);
	void UpdateLights(Scene& p_scene);
	void RenderCameras();
	void RenderLights();

private:
	Context& m_context;

	Material m_defaultMaterial;
	Material m_emptyMaterial;
	Material m_gridMaterial;
	Material m_cameraMaterial;
	Material m_outlineMaterial;
	Material m_gizmoArrowMaterial;
	Material m_gizmoBallMaterial;
	Material m_gizmoPickingMaterial;
	Material m_actorPickingMaterial;
	Material m_lightMaterial;
};

