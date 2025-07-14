#include "Toolbar.h"
#include "../EditorAction.h"

#include <Mino/Widget/Spacing.h>

Panel::Toolbar::Toolbar(const std::string& p_title) :
	PanelWindow(p_title)
{
	m_playButton = &CreateWidget<Widget::ButtonImage>("play", EDITOR_CONTEXT(editorResource)->GetTexture("Button_Play")->id, FVector2 { 20, 20 });
	m_pauseButton = &CreateWidget<Widget::ButtonImage>("pause", EDITOR_CONTEXT(editorResource)->GetTexture("Button_Pause")->id, FVector2 { 20, 20 });
	m_stopButton = &CreateWidget<Widget::ButtonImage>("stop", EDITOR_CONTEXT(editorResource)->GetTexture("Button_Stop")->id, FVector2 { 20, 20 });
	m_nextButton = &CreateWidget<Widget::ButtonImage>("next", EDITOR_CONTEXT(editorResource)->GetTexture("Button_Next")->id, FVector2 { 20, 20 });

	CreateWidget<Widget::Spacing>(0).lineBreak = false;
	auto& refreshButton = CreateWidget<Widget::ButtonImage>("refresh", EDITOR_CONTEXT(editorResource)->GetTexture("Button_Refresh")->id, FVector2 { 20, 20 });

	m_playButton->lineBreak = false;
	m_pauseButton->lineBreak = false;
	m_stopButton->lineBreak = false;
	m_nextButton->lineBreak = false;
	refreshButton.lineBreak = false;

	m_playButton->ClickedEvent += EDITOR_BIND(StartPlaying);
	m_pauseButton->ClickedEvent += EDITOR_BIND(PauseGame);
	m_stopButton->ClickedEvent += EDITOR_BIND(StopPlaying);
	m_nextButton->ClickedEvent += EDITOR_BIND(NextFrame);
	refreshButton.ClickedEvent += EDITOR_BIND(RefreshScripts);
}

void Panel::Toolbar::_Draw_Impl()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
	PanelWindow::_Draw_Impl();
	//ImGui::PopStyleVar();
}
