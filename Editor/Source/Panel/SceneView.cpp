#include <optional>

#include "SceneView.h"
#include "../EditorAction.h"

Panel::SceneView::SceneView(const std::string& p_title) :
	AViewControllable(p_title),
	m_sceneManager(EDITOR_CONTEXT(sceneManager))
{
	m_camera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	m_camera.SetFar(5000.0f);
}

void Panel::SceneView::Update(float p_deltaTime)
{
	AViewControllable::Update(p_deltaTime);

	if (IsFocused() && !m_cameraController.IsRightMousePressed())
	{
		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(EKey::KEY_W))
		{
			m_currentOperation = EGizmoOperation::TRANSLATE;
		}

		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(EKey::KEY_E))
		{
			m_currentOperation = EGizmoOperation::ROTATE;
		}

		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(EKey::KEY_R))
		{
			m_currentOperation = EGizmoOperation::SCALE;
		}
	}
}

void Panel::SceneView::_Render_Impl()
{
	PrepareCamera();

	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);
	HandleActorPicking();
	baseRenderer.ApplyStateMask(glState);
	RenderScene(glState);
	baseRenderer.ApplyStateMask(glState);
}

void Panel::SceneView::RenderScene(uint8_t p_defaultRenderState)
{
	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();
	auto& currentScene = *m_sceneManager.GetCurrentScene();

	m_editorRenderer.UpdateLights(currentScene);

	m_fbo.Bind();

	baseRenderer.SetStencilMask(0xFF);
	baseRenderer.Clear(m_camera);
	baseRenderer.SetStencilMask(0x00);

	m_editorRenderer.RenderGrid(m_cameraPosition, FVector3{ 0.176f, 0.176f, 0.176f });
	m_editorRenderer.RenderCameras();

	m_editorRenderer.RenderScene(m_cameraPosition, m_camera);

	m_editorRenderer.RenderLights();

	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());

		baseRenderer.ApplyStateMask(p_defaultRenderState);
		m_editorRenderer.RenderActorOutlinePass(selectedActor, true);

		baseRenderer.ApplyStateMask(p_defaultRenderState);
		baseRenderer.Clear(false, true, false);

		int highlightedAxis = -1;

		if (m_highlightedGizmoDirection.has_value())
		{
			highlightedAxis = static_cast<int>(m_highlightedGizmoDirection.value());
		}

		m_editorRenderer.RenderGizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation(), m_currentOperation, false, highlightedAxis);
	}

	if (m_highlightedActor.has_value())
	{
		baseRenderer.ApplyStateMask(p_defaultRenderState);
		m_editorRenderer.RenderActorOutlinePass(m_highlightedActor.value().get(), false);
	}

	m_fbo.Unbind();
}

void Panel::SceneView::RenderSceneForActorPicking()
{
	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();

	auto [winWidth, winHeight] = GetSafeSize();

	m_actorPickingFramebuffer.Resize(winWidth, winHeight);
	m_actorPickingFramebuffer.Bind();
	baseRenderer.SetClearColor(1.0f, 1.0f, 1.0f);
	baseRenderer.Clear();
	m_editorRenderer.RenderSceneForActorPicking();

	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());
		baseRenderer.Clear(false, true, false);
		m_editorRenderer.RenderGizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation(), m_currentOperation, true);
	}

	m_actorPickingFramebuffer.Unbind();
}


void Panel::SceneView::HandleActorPicking()
{
	auto& inputManager = *EDITOR_CONTEXT(inputManager);

	if (inputManager.IsMouseButtonReleased(EMouseButton::MOUSE_BUTTON_LEFT))
	{
		m_gizmoBehaviour.StopPicking();
	}

	if (IsHovered())
	{
		RenderSceneForActorPicking();

		auto [mouseX, mouseY] = inputManager.GetMousePosition();
		mouseX -= m_position.x;
		mouseY -= m_position.y;
		mouseY = GetSafeSize().second - mouseY + 25;

		m_actorPickingFramebuffer.Bind();
		uint8_t pixel[3];
		EDITOR_CONTEXT(renderer)->ReadPixels(static_cast<int>(mouseX), static_cast<int>(mouseY), 1, 1, EPixelDataFormat::RGB, EPixelDataType::UNSIGNED_BYTE, pixel);
		m_actorPickingFramebuffer.Unbind();

		uint32_t actorID = (0 << 24) | (pixel[2] << 16) | (pixel[1] << 8) | (pixel[0] << 0);
		auto actorUnderMouse = EDITOR_CONTEXT(sceneManager).GetCurrentScene()->FindActorByID(actorID);
		auto direction = m_gizmoBehaviour.IsPicking() ? m_gizmoBehaviour.GetDirection() : EDITOR_EXEC(IsAnyActorSelected()) && pixel[0] == 255 && pixel[1] == 255 && pixel[2] >= 252 && pixel[2] <= 254 ? static_cast<GizmoBehaviour::EDirection>(pixel[2] - 252) : std::optional<GizmoBehaviour::EDirection>{};

		m_highlightedActor = {};
		m_highlightedGizmoDirection = {};

		if (!m_cameraController.IsRightMousePressed())
		{
			if (direction.has_value())
			{
				m_highlightedGizmoDirection = direction;
			}
			else if (actorUnderMouse != nullptr)
			{
				m_highlightedActor = std::ref(*actorUnderMouse);
			}
		}

		if (inputManager.IsMouseButtonPressed(EMouseButton::MOUSE_BUTTON_LEFT) && !m_cameraController.IsRightMousePressed())
		{
			if (direction.has_value())
			{
				m_gizmoBehaviour.StartPicking(EDITOR_EXEC(GetSelectedActor()), m_cameraPosition, m_currentOperation, direction.value());
			}
			else
			{
				if (actorUnderMouse)
				{
					EDITOR_EXEC(SelectActor(*actorUnderMouse));
				}
				else
				{
					EDITOR_EXEC(UnselectActor());
				}
			}
		}
	}

	if (m_gizmoBehaviour.IsPicking())
	{
		auto mousePosition = EDITOR_CONTEXT(inputManager)->GetMousePosition();

		auto [winWidth, winHeight] = GetSafeSize();

		m_gizmoBehaviour.SetCurrentMouse({ static_cast<float>(mousePosition.first), static_cast<float>(mousePosition.second) });
		m_gizmoBehaviour.ApplyOperation(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), { static_cast<float>(winWidth), static_cast<float>(winHeight) });
	}
}

