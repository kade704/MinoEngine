#pragma once

#include "PanelWindow.h"
#include "../Camera.h"
#include "../Widget/Image.h"
#include "../Buffer/FrameBuffer.h"

class EditorRenderer;

namespace Panel
{
	class AView : public PanelWindow
	{
	public:
		AView(const std::string& p_title);

		virtual void Update(float p_deltaTime);

		void _Draw_Impl() override;

		virtual void _Render_Impl() = 0;

		void Render();

		std::pair<uint16_t, uint16_t> GetSafeSize() const;

	protected:
		void FillEngineUBO();
		void PrepareCamera();

	protected:
		EditorRenderer& m_editorRenderer;
		Camera m_camera;
		FVector3 m_cameraPosition;
		FQuaternion m_cameraRotation;
		Widget::Image* m_image;
		Buffer::FrameBuffer m_fbo;
	};
}
