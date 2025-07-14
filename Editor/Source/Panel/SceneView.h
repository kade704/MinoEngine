#pragma once

#include <optional>

#include "AViewControllable.h"

#include <Mino/Manager/SceneManager.h>
#include <Mino/Buffer/FrameBuffer.h>
#include <Mino/GizmoBehaviour.h>

namespace Panel
{
	class SceneView : public AViewControllable
	{
	public:
		SceneView(const std::string& p_title);

		virtual void Update(float p_deltaTime) override;

		virtual void _Render_Impl() override;

		void RenderScene(uint8_t p_defaultRenderState);

		void HandleActorPicking();

		void RenderSceneForActorPicking();

	private:
		SceneManager& m_sceneManager;
		GizmoBehaviour m_gizmoBehaviour;
		EGizmoOperation m_currentOperation = EGizmoOperation::TRANSLATE;
		Buffer::FrameBuffer m_actorPickingFramebuffer;
		std::optional<std::reference_wrapper<Actor>> m_highlightedActor;
		std::optional<GizmoBehaviour::EDirection> m_highlightedGizmoDirection;
	};
}
