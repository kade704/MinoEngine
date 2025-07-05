#include "AView.h"
#include "../EditorAction.h"
#include "../Renderer.h"

Panel::AView::AView(const std::string& p_title) :
	PanelWindow(p_title),
	m_editorRenderer(EDITOR_RENDERER())
{
	m_cameraPosition = FVector3(0, 0, 0);
	m_cameraRotation = FQuaternion::Identity;

	m_image = &CreateWidget<Widget::Image>(m_fbo.GetTextureID(), FVector2(0.f, 0.f));
}

void Panel::AView::Update(float p_deltaTime)
{
	auto [winWidth, winHeight] = GetSafeSize();
	m_image->size = FVector2(static_cast<float>(winWidth), static_cast<float>(winHeight));
	m_fbo.Resize(winWidth, winHeight);
}

void Panel::AView::_Draw_Impl()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	PanelWindow::_Draw_Impl();
	ImGui::PopStyleVar();
}

void Panel::AView::Render()
{
	FillEngineUBO();
	auto [winWidth, winHeight] = GetSafeSize();
	EDITOR_CONTEXT(renderer)->SetViewport(0, 0, winWidth, winHeight);
	_Render_Impl();
}

std::pair<uint16_t, uint16_t> Panel::AView::GetSafeSize() const
{
	auto result = GetSize() - FVector2(0.f, 0.25f);
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

void Panel::AView::FillEngineUBO()
{
	auto& engineUBO = *EDITOR_CONTEXT(engineUBO);

	auto [winWidth, winHeight] = GetSafeSize();

	size_t offset = sizeof(FMatrix4);
	engineUBO.SetSubData(FMatrix4::Transpose(m_camera.GetViewMatrix()), std::ref(offset));
	engineUBO.SetSubData(FMatrix4::Transpose(m_camera.GetProjectionMatrix()), std::ref(offset));
	engineUBO.SetSubData(m_cameraPosition, std::ref(offset));
}

void Panel::AView::PrepareCamera()
{
	auto [winWidth, winHeight] = GetSafeSize();
	m_camera.CacheMatrices(winWidth, winHeight, m_cameraPosition, m_cameraRotation);
}