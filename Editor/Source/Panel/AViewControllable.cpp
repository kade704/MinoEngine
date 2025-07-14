#include "AViewControllable.h"

Panel::AViewControllable::AViewControllable(const std::string& p_title) :
	AView(p_title),
	m_cameraController(*this, m_camera, m_cameraPosition, m_cameraRotation)
{
}

void Panel::AViewControllable::Update(float p_deltaTime)
{
	m_cameraController.HandleInputs(p_deltaTime);

	AView::Update(p_deltaTime);
}

CameraController& Panel::AViewControllable::GetCameraController()
{
	return m_cameraController;
}
