#pragma once

#include "AView.h"
#include "../CameraController.h"

namespace Panel
{
	class AViewControllable : public AView
	{
	public:
		AViewControllable(const std::string& p_title);

		virtual void Update(float p_deltaTime) override;

		CameraController& GetCameraController();

	protected:
		CameraController m_cameraController;
	};
}
