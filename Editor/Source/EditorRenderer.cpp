#include "EditorRenderer.h"
#include "Panel/GameView.h"
#include "EditorAction.h"
#include "GizmoBehaviour.h"

#include <Mino/Component/CMaterialRenderer.h>

const FVector3 FRUSTUM_COLOR = { 1.0f, 1.0f, 1.0f };

EditorRenderer::EditorRenderer(Context& p_context) :
	m_context(p_context)
{
	// m_context.renderer->SetCapability(ERenderingCapability::STENCIL_TEST, true);
	// m_context.renderer->SetStencilOperations(EOperation::KEEP, EOperation::KEEP, EOperation::REPLACE);
	// m_context.renderer->SetStencilAlgorithm(EComparaisonAlgorithm::ALWAYS, 1, 0xFF);

	InitMaterials();

	m_context.renderer->RegisterModelMatrixSender([this](const FMatrix4& p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(FMatrix4::Transpose(p_modelMatrix), 0);
	});

	m_context.renderer->RegisterUserMatrixSender([this](const FMatrix4& p_userMatrix)
	{
		m_context.engineUBO->SetSubData
		(
			p_userMatrix,

			sizeof(FMatrix4) +
			sizeof(FMatrix4) +
			sizeof(FMatrix4) +
			sizeof(FVector3) +
			sizeof(float)
		);
	});

	
}

void EditorRenderer::InitMaterials()
{
	m_defaultMaterial.SetShader(m_context.editorResource->GetShader("Standard"));
	m_defaultMaterial.Set("u_Diffuse", FVector4(1.f, 1.f, 1.f, 1.f));
	m_defaultMaterial.Set("u_Shininess", 100.0f);
	m_defaultMaterial.Set<Texture*>("u_DiffuseMap", nullptr);

	m_emptyMaterial.SetShader(m_context.editorResource->GetShader("Unlit"));
	m_emptyMaterial.Set("u_Diffuse", FVector4(1.f, 0.f, 1.f, 1.0f));
	m_emptyMaterial.Set<Texture*>("u_DiffuseMap", nullptr);

	m_gridMaterial.SetShader(m_context.editorResource->GetShader("Grid"));
	m_gridMaterial.SetBlendable(true);
	m_gridMaterial.SetBackfaceCulling(false);
	m_gridMaterial.SetDepthTest(false);

	m_cameraMaterial.SetShader(m_context.editorResource->GetShader("Lambert"));
	m_cameraMaterial.Set("u_Diffuse", FVector4(0.0f, 0.3f, 0.7f, 1.0f));
	m_cameraMaterial.Set<Texture*>("u_DiffuseMap", nullptr);

	m_lightMaterial.SetShader(m_context.editorResource->GetShader("Billboard"));
	m_lightMaterial.Set("u_Diffuse", FVector4(1.f, 1.f, 0.5f, 0.5f));
	m_lightMaterial.SetBackfaceCulling(false);
	m_lightMaterial.SetBlendable(true);
	m_lightMaterial.SetDepthTest(false);

	m_outlineMaterial.SetShader(m_context.editorResource->GetShader("Unlit"));
	m_outlineMaterial.Set("u_Diffuse", FVector4(1.f, 0.7f, 0.f, 1.0f));
	m_outlineMaterial.SetDepthTest(false);

	m_gizmoArrowMaterial.SetShader(m_context.editorResource->GetShader("Gizmo"));
	m_gizmoArrowMaterial.SetGPUInstances(3);
	m_gizmoArrowMaterial.Set("u_IsBall", false);
	m_gizmoArrowMaterial.Set("u_IsPickable", false);

	m_gizmoBallMaterial.SetShader(m_context.editorResource->GetShader("Gizmo"));
	m_gizmoBallMaterial.Set("u_IsBall", true);
	m_gizmoBallMaterial.Set("u_IsPickable", false);

	m_gizmoPickingMaterial.SetShader(m_context.editorResource->GetShader("Gizmo"));
	m_gizmoPickingMaterial.SetGPUInstances(3);
	m_gizmoPickingMaterial.Set("u_IsBall", false);
	m_gizmoPickingMaterial.Set("u_IsPickable", true);

	m_actorPickingMaterial.SetShader(m_context.editorResource->GetShader("Unlit"));
	m_actorPickingMaterial.Set("u_Diffuse", FVector4(1.f, 1.f, 1.f, 1.0f));
	m_actorPickingMaterial.Set<Texture*>("u_DiffuseMap", nullptr);
	m_actorPickingMaterial.SetFrontfaceCulling(false);
	m_actorPickingMaterial.SetBackfaceCulling(false);
}

void EditorRenderer::PreparePickingMaterial(Actor& p_actor, Material& p_material)
{
	uint32_t actorID = static_cast<uint32_t>(p_actor.GetID());

	auto bytes = reinterpret_cast<uint8_t*>(&actorID);
	auto color = FVector4{ bytes[0] / 255.0f, bytes[1] / 255.0f, bytes[2] / 255.0f, 1.0f };

	p_material.Set("u_Diffuse", color);
}

FMatrix4 EditorRenderer::CalculateCameraModelMatrix(Actor& p_actor)
{
	auto translation = FMatrix4::Translation(p_actor.transform.GetWorldPosition());
	auto rotation = FQuaternion::ToMatrix4(p_actor.transform.GetWorldRotation());

	return translation * rotation;
}

void EditorRenderer::RenderScene(const FVector3& p_cameraPosition, const Camera& p_camera)
{
	m_context.lightSSBO->Bind(0);
	m_context.renderer->RenderScene(*m_context.sceneManager.GetCurrentScene(), p_camera, p_cameraPosition, &m_emptyMaterial);
	m_context.lightSSBO->Unbind();
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
						if (!material || !material->GetShader())
							material = &m_emptyMaterial;
					}

					if (material)
					{
						m_context.renderer->DrawMesh(*mesh, m_actorPickingMaterial, &modelMatrix);
					}
				}
			}
		}
	}

	for (auto camera : m_context.sceneManager.GetCurrentScene()->GetFastAccessComponents().cameras)
	{
		auto& actor = camera->owner;

		PreparePickingMaterial(actor, m_actorPickingMaterial);
		auto& model = *m_context.editorResource->GetModel("Camera");
		auto modelMatrix = CalculateCameraModelMatrix(actor);

		m_context.renderer->DrawModelWithSingleMaterial(model, m_actorPickingMaterial, &modelMatrix);
	}

	m_context.renderer->Clear(false, true, false);

	m_lightMaterial.SetDepthTest(true);
	m_lightMaterial.Set<float>("u_Scale", 0.05f);
	m_lightMaterial.Set<Texture*>("u_DiffuseMap", nullptr);

	for (auto light : m_context.sceneManager.GetCurrentScene()->GetFastAccessComponents().lights)
	{
		auto& actor = light->owner;

		PreparePickingMaterial(actor, m_lightMaterial);
		auto& model = *m_context.editorResource->GetModel("Vertical_Plane");
		auto modelMatrix = FMatrix4::Translation(actor.transform.GetWorldPosition());
		m_context.renderer->DrawModelWithSingleMaterial(model, m_lightMaterial, &modelMatrix);
	}
}

void EditorRenderer::RenderCameras()
{
	for (auto camera : m_context.sceneManager.GetCurrentScene()->GetFastAccessComponents().cameras)
	{
		auto& actor = camera->owner;

		auto& model = *m_context.editorResource->GetModel("Camera");
		auto modelMatrix = CalculateCameraModelMatrix(actor);

		m_context.renderer->DrawModelWithSingleMaterial(model, m_cameraMaterial, &modelMatrix);
	}
}

void EditorRenderer::RenderLights()
{
	m_lightMaterial.SetDepthTest(false);
	m_lightMaterial.Set<float>("u_Scale", 0.05f);

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

void EditorRenderer::RenderGizmo(const FVector3& p_position, const FQuaternion& p_rotation, EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis)
{
	FMatrix4 model = FMatrix4::Translation(p_position) * FQuaternion::ToMatrix4(FQuaternion::Normalize(p_rotation));

	Model* arrowModel = nullptr;

	if (!p_pickable)
	{
		FMatrix4 sphereModel = model * FMatrix4::Scaling({ 0.1f, 0.1f, 0.1f });
		m_context.renderer->DrawModelWithSingleMaterial(*m_context.editorResource->GetModel("Sphere"), m_gizmoBallMaterial, &sphereModel);
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
	float outlineWidth = p_isSelected ? 5.0f : 2.5f;

	m_outlineMaterial.Set("u_Diffuse", p_isSelected ? FVector4(1.f, 0.7f, 0.f, 1.0f) : FVector4(0.5f, 0.3f, 0.f, 1.0f));

	if (auto modelRenderer = p_actor.GetComponent<Component::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
	{
		RenderModelOutline(p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel(), outlineWidth);
	}

	if (auto cameraComponent = p_actor.GetComponent<Component::CCamera>(); cameraComponent)
	{
		auto model = CalculateCameraModelMatrix(p_actor);

		RenderModelOutline(model, *m_context.editorResource->GetModel("Camera"), outlineWidth);

		if (p_isSelected)
		{
			RenderCameraFrustum(*cameraComponent);
		}
	}

	for (auto& child : p_actor.GetChildren())
	{
		RenderActorOutlinePass(*child, p_isSelected);
	}
}

void DrawFrustumLines(ShapeDrawer& p_drawer,
                      const FVector3& pos,
                      const FVector3& forward,
                      float near,
                      const float far,
                      const FVector3& a,
                      const FVector3& b,
                      const FVector3& c,
                      const FVector3& d,
                      const FVector3& e,
                      const FVector3& f,
                      const FVector3& g,
                      const FVector3& h)
{
    // Convenient lambda to draw a frustum line
    auto draw = [&](const FVector3& p_start, const FVector3& p_end, const float planeDistance)
    {
        auto offset = pos + forward * planeDistance;
        auto start = offset + p_start;
        auto end = offset + p_end;
        p_drawer.DrawLine(start, end, FRUSTUM_COLOR);
    };

    // Draw near plane
    draw(a, b, near);
    draw(b, d, near);
    draw(d, c, near);
    draw(c, a, near);

    // Draw far plane
    draw(e, f, far);
    draw(f, h, far);
    draw(h, g, far);
    draw(g, e, far);

    // Draw lines between near and far planes
    draw(a + forward * near, e + forward * far, 0);
    draw(b + forward * near, f + forward * far, 0);
    draw(c + forward * near, g + forward * far, 0);
    draw(d + forward * near, h + forward * far, 0);
}

void EditorRenderer::RenderCameraFrustum(Component::CCamera &p_camera)
{
	auto& gameView = EDITOR_PANEL(Panel::GameView, "Game View");
	auto gameViewSize = gameView.GetSafeSize();

	if (gameViewSize.first == 0 || gameViewSize.second == 0)
	{
		gameViewSize = { 16, 9 };
	}

	const auto& owner = p_camera.owner;
    auto& camera = p_camera.GetCamera();

    const auto& cameraPos = owner.transform.GetWorldPosition();
    const auto& cameraRotation = owner.transform.GetWorldRotation();
    const auto& cameraForward = owner.transform.GetWorldForward();

    camera.CacheMatrices(gameViewSize.first, gameViewSize.second, cameraPos, cameraRotation);
    const auto proj = FMatrix4::Transpose(camera.GetProjectionMatrix());
    const auto near = camera.GetNear();
    const auto far = camera.GetFar();

    const auto nLeft = near * (proj.data[2] - 1.0f) / proj.data[0];
    const auto nRight = near * (1.0f + proj.data[2]) / proj.data[0];
    const auto nTop = near * (1.0f + proj.data[6]) / proj.data[5];
    const auto nBottom = near * (proj.data[6] - 1.0f) / proj.data[5];

    const auto fLeft = far * (proj.data[2] - 1.0f) / proj.data[0];
    const auto fRight = far * (1.0f + proj.data[2]) / proj.data[0];
    const auto fTop = far * (1.0f + proj.data[6]) / proj.data[5];
    const auto fBottom = far * (proj.data[6] - 1.0f) / proj.data[5];

    auto a = cameraRotation * FVector3{ nLeft, nTop, 0 };
    auto b = cameraRotation * FVector3{ nRight, nTop, 0 };
    auto c = cameraRotation * FVector3{ nLeft, nBottom, 0 };
    auto d = cameraRotation * FVector3{ nRight, nBottom, 0 };
    auto e = cameraRotation * FVector3{ fLeft, fTop, 0 };
    auto f = cameraRotation * FVector3{ fRight, fTop, 0 };
    auto g = cameraRotation * FVector3{ fLeft, fBottom, 0 };
    auto h = cameraRotation * FVector3{ fRight, fBottom, 0 };

    DrawFrustumLines(*m_context.shapeDrawer, cameraPos, cameraForward, near, far, a, b, c, d, e, f, g, h);
}

void EditorRenderer::RenderGrid(const FVector3& p_viewPos, const FVector3& p_color)
{
    constexpr float gridSize = 5000.0f;

    FMatrix4 model = FMatrix4::Translation({ p_viewPos.x, 0.0f, p_viewPos.z }) * FMatrix4::Scaling({ gridSize * 2.0f, 1.f, gridSize * 2.0f });
	m_gridMaterial.Set("u_Color", p_color);
	m_context.renderer->DrawModelWithSingleMaterial(*m_context.editorResource->GetModel("Plane"), m_gridMaterial, &model);

    m_context.shapeDrawer->DrawLine(FVector3(-gridSize + p_viewPos.x, 0.0f, 0.0f), FVector3(gridSize + p_viewPos.x, 0.0f, 0.0f), FVector3(1.0f, 0.0f, 0.0f), 1.0f);
    m_context.shapeDrawer->DrawLine(FVector3(0.0f, -gridSize + p_viewPos.y, 0.0f), FVector3(0.0f, gridSize + p_viewPos.y, 0.0f), FVector3(0.0f, 1.0f, 0.0f), 1.0f);
    m_context.shapeDrawer->DrawLine(FVector3(0.0f, 0.0f, -gridSize + p_viewPos.z), FVector3(0.0f, 0.0f, gridSize + p_viewPos.z), FVector3(0.0f, 0.0f, 1.0f), 1.0f);
}

void EditorRenderer::UpdateLights(Scene& p_scene)
{
	auto lightMatrices = m_context.renderer->FindLightMatrices(p_scene);
	m_context.lightSSBO->SendBlocks<FMatrix4>(lightMatrices.data(), lightMatrices.size() * sizeof(FMatrix4));
}


