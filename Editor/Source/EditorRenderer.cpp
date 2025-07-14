#include "EditorRenderer.h"

#include <Mino/Loader/ShaderLoader.h>
#include <Mino/Component/CMaterialRenderer.h>
#include <Mino/GizmoBehaviour.h>

EditorRenderer::EditorRenderer(Context& p_context) :
	m_context(p_context)
{
	m_context.renderer->RegisterModelMatrixSender([this](const FMatrix4& p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(FMatrix4::Transpose(p_modelMatrix), 0);
	});


	m_outlineMaterial.SetShader(m_context.shaderManager.GetResource("Contents/editor/unlit.shader"));
	m_outlineMaterial.Set("u_Diffuse", FVector4(1.f, 0.7f, 0.f, 1.0f));
	m_outlineMaterial.SetDepthTest(false);

	m_gizmoArrowMaterial.SetShader(m_context.editorResource->GetShader("Gizmo"));
	m_gizmoArrowMaterial.SetGPUInstances(3);
	m_gizmoArrowMaterial.Set("u_IsPickable", false);
	m_gizmoArrowMaterial.Set("u_Scale", 0.001f);

	m_gizmoPickingMaterial.SetShader(m_context.editorResource->GetShader("Gizmo"));
	m_gizmoPickingMaterial.SetGPUInstances(3);
	m_gizmoPickingMaterial.Set("u_IsPickable", true);

	m_actorPickingMaterial.SetShader(m_context.shaderManager.GetResource("Contents/editor/unlit.shader"));
	m_actorPickingMaterial.Set("u_Diffuse", FVector4(1.f, 1.f, 1.f, 1.0f));
	m_actorPickingMaterial.SetFrontfaceCulling(false);
	m_actorPickingMaterial.SetBackfaceCulling(false);

	m_lightMaterial.SetShader(m_context.editorResource->GetShader("Billboard"));
	m_lightMaterial.Set("u_Diffuse", FVector4(1.f, 1.f, 0.5f, 0.5f));
	m_lightMaterial.Set("u_Scale", 0.001f);
	m_lightMaterial.SetBackfaceCulling(false);
	m_lightMaterial.SetBlendable(true);
	m_lightMaterial.SetDepthTest(false);
}

void EditorRenderer::RenderModelOutline(const FMatrix4& p_worldMatrix, Model& p_model, float p_width)
{
	m_context.renderer->SetStencilAlgorithm(EComparaisonAlgorithm::NOTEQUAL, 1, 0xFF);
	m_context.renderer->SetRasterizationMode(ERasterizationMode::LINE);
	m_context.renderer->SetRasterizationLinesWidth(p_width);
	m_context.renderer->DrawModelWithSingleMaterial(p_model, m_outlineMaterial, &p_worldMatrix);
	m_context.renderer->SetRasterizationLinesWidth(1.f);
	m_context.renderer->SetRasterizationMode(ERasterizationMode::FILL);
	m_context.renderer->SetStencilAlgorithm(EComparaisonAlgorithm::ALWAYS, 1, 0xFF);
}

void EditorRenderer::RenderActorOutlinePass(Actor& p_actor, bool p_isSelected)
{
	m_outlineMaterial.Set("u_Diffuse", p_isSelected ? FVector4(1.f, 0.7f, 0.f, 1.0f) : FVector4(0.5f, 0.3f, 0.f, 1.0f));

	if (auto modelRenderer = p_actor.GetComponent<Component::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
	{
		RenderModelOutline(p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel(), 1.0f);
	}

	for (auto& child : p_actor.GetChildren())
	{
		RenderActorOutlinePass(*child, false);
	}
}

void EditorRenderer::RenderGizmo(const FVector3& p_position, const FQuaternion& p_rotation, EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis)
{
	FMatrix4 model = FMatrix4::Translation(p_position) * FQuaternion::ToMatrix4(FQuaternion::Normalize(p_rotation));

	Model* arrowModel = nullptr;

	if (!p_pickable)
	{
		m_gizmoArrowMaterial.Set("u_HighlightedAxis", p_highlightedAxis);

		switch (p_operation)
		{
		case EGizmoOperation::TRANSLATE:
			arrowModel = m_context.editorResource->GetModel("Arrow_Translate");
			break;
		case EGizmoOperation::ROTATE:
			arrowModel = m_context.editorResource->GetModel("Arrow_Rotate");
			break;
		case EGizmoOperation::SCALE:
			arrowModel = m_context.editorResource->GetModel("Arrow_Scale");
			break;
		}
	}
	else
	{
		arrowModel = m_context.editorResource->GetModel("Arrow_Picking");
	}

	if (arrowModel)
	{
		m_context.renderer->DrawModelWithSingleMaterial(*arrowModel, p_pickable ? m_gizmoPickingMaterial : m_gizmoArrowMaterial, &model);
	}
}

void EditorRenderer::RenderSceneForActorPicking()
{
	auto& scene = *m_context.sceneManager.GetCurrentScene();

	for (auto modelRenderer : scene.GetFastAccessComponents().modelRenderers)
	{
		auto& actor = modelRenderer->owner;

		if (auto model = modelRenderer->GetModel())
		{
			if (auto materialRenderer = modelRenderer->owner.GetComponent<Component::CMaterialRenderer>())
			{
				const Component::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();
				const auto& modelMatrix = actor.transform.GetWorldMatrix();

				PreparePickingMaterial(actor, m_actorPickingMaterial);

				for (auto mesh : model->GetMeshes())
				{
					Material* material = nullptr;

					if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
					{
						material = materials.at(mesh->GetMaterialIndex());
					}

					if (material)
					{
						m_context.renderer->DrawMesh(*mesh, m_actorPickingMaterial, &modelMatrix);
					}
				}
			}
		}
	}

	m_context.renderer->Clear(false, true, false);

	auto lightTexture = m_context.editorResource->GetTexture("White_Background");

	m_lightMaterial.SetDepthTest(true);
	m_lightMaterial.Set<Texture*>("u_DiffuseMap", lightTexture);

	for (auto light : m_context.sceneManager.GetCurrentScene()->GetFastAccessComponents().lights)
	{
		auto& actor = light->owner;

		PreparePickingMaterial(actor, m_lightMaterial);
		auto& model = *m_context.editorResource->GetModel("Vertical_Plane");
		auto modelMatrix = FMatrix4::Translation(actor.transform.GetWorldPosition());
		m_context.renderer->DrawModelWithSingleMaterial(model, m_lightMaterial, &modelMatrix);
	}
}

void EditorRenderer::PreparePickingMaterial(Actor& p_actor, Material& p_material)
{
	uint32_t actorID = static_cast<uint32_t>(p_actor.GetID());

	auto bytes = reinterpret_cast<uint8_t*>(&actorID);
	auto color = FVector4{ bytes[0] / 255.0f, bytes[1] / 255.0f, bytes[2] / 255.0f, 1.0f };

	p_material.Set("u_Diffuse", color);
}

void EditorRenderer::UpdateLights(Scene& p_scene)
{
	auto lightMatrices = m_context.renderer->FindLightMatrices(p_scene);
	m_context.lightSSBO->SendBlocks<FMatrix4>(lightMatrices.data(), lightMatrices.size() * sizeof(FMatrix4));
}

void EditorRenderer::RenderLights()
{
	m_lightMaterial.SetDepthTest(false);

	for (auto light : m_context.sceneManager.GetCurrentScene()->GetFastAccessComponents().lights)
	{
		auto& actor = light->owner;

		auto& model = *m_context.editorResource->GetModel("Vertical_Plane");
		auto modelMatrix = FMatrix4::Translation(actor.transform.GetWorldPosition());

		Texture* texture = nullptr;

		switch (static_cast<Light::Type>(static_cast<int>(light->GetData().type)))
		{
		case Light::Type::POINT:				texture = m_context.editorResource->GetTexture("Bill_Point_Light");			break;
		case Light::Type::SPOT:				texture = m_context.editorResource->GetTexture("Bill_Spot_Light");				break;
		case Light::Type::DIRECTIONAL:		texture = m_context.editorResource->GetTexture("Bill_Directional_Light");		break;
		case Light::Type::AMBIENT_BOX:		texture = m_context.editorResource->GetTexture("Bill_Ambient_Box_Light");		break;
		case Light::Type::AMBIENT_SPHERE:	texture = m_context.editorResource->GetTexture("Bill_Ambient_Sphere_Light");	break;
		}

		const auto& lightColor = light->GetColor();
		m_lightMaterial.Set<Texture*>("u_DiffuseMap", texture);
		m_lightMaterial.Set<FVector4>("u_Diffuse", FVector4(lightColor.x, lightColor.y, lightColor.z, 0.75f));
		m_context.renderer->DrawModelWithSingleMaterial(model, m_lightMaterial, &modelMatrix);
	}
}
