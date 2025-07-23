#include "Editor.h"
#include "Panel/Profiler.h"
#include "Panel/SceneView.h"
#include "Panel/Hierarchy.h"
#include "Panel/Inspector.h"
#include "Panel/AssetBrowser.h"
#include "Panel/Menubar.h"
#include "Panel/Toolbar.h"
#include "Panel/Console.h"
#include "Panel/GameView.h"

#include <Mino/ProfilerSpy.h>

Editor::Editor(Context& p_context) :
	m_context(p_context),
	m_editorRenderer(p_context),
	m_panelManager(m_canvas),
	m_editorAction(m_context, m_editorRenderer, m_panelManager)
{
	SetupUI();
}

Editor::~Editor()
{
}

void Editor::SetupUI()
{
	m_panelManager.CreatePanel<Panel::Menubar>("Menubar");
	m_panelManager.CreatePanel<Panel::Profiler>("Profiler", 0.25f);
	m_panelManager.CreatePanel<Panel::Hierarchy>("Hierarchy");
	m_panelManager.CreatePanel<Panel::SceneView>("Scene View");
	m_panelManager.CreatePanel<Panel::Inspector>("Inspector");
	m_panelManager.CreatePanel<Panel::AssetBrowser>("Asset Browser");
	m_panelManager.CreatePanel<Panel::Toolbar>("Toolbar");
	m_panelManager.CreatePanel<Panel::Console>("Console");
	m_panelManager.CreatePanel<Panel::GameView>("Game View");

	m_context.uiManager->SetCanvas(m_canvas);
}

void Editor::PreUpdate()
{
	PROFILER_SPY("Editor PreUpdate");

	m_context.device->PollEvents();
	m_context.renderer->SetClearColor(0.1f, 0.1f, 0.1f);
	m_context.renderer->Clear();	
}

void Editor::Update(float p_deltaTime)
{
	m_context.uiManager->Render();
	
	UpdateCurrentEditorMode(p_deltaTime);
	PrepareRendering(p_deltaTime);
	UpdateEditorPanels(p_deltaTime);
	RenderViews(p_deltaTime);
	RenderEditorUI(p_deltaTime);
	m_editorAction.ExecuteDelayedActions();
}

void Editor::PostUpdate()
{
	PROFILER_SPY("Editor PostUpdate");
	m_context.inputManager->ClearEvents();
	m_context.window->SwapBuffers();
}

void Editor::UpdateCurrentEditorMode(float p_deltaTime)
{
	if (auto editorMode = m_editorAction.GetCurrentEditorMode(); editorMode == EditorAction::EEditorMode::PLAY || editorMode == EditorAction::EEditorMode::FRAME_BY_FRAME)
		UpdatePlayMode(p_deltaTime);
	else
		UpdateEditMode(p_deltaTime);

	m_context.sceneManager.GetCurrentScene()->CollectGarbages();
	//m_context.sceneManager.Update();
}

void Editor::UpdatePlayMode(float p_deltaTime)
{
	auto currentScene = m_context.sceneManager.GetCurrentScene();

	{
		PROFILER_SPY("Update");
		currentScene->Update(p_deltaTime);
	}

	if (m_context.inputManager->IsKeyPressed(EKey::KEY_ESCAPE))
		m_editorAction.StopPlaying();
}

void Editor::UpdateEditMode(float p_deltaTime)
{
	if (m_context.inputManager->IsKeyPressed(EKey::KEY_F5))
		m_editorAction.StartPlaying();
}

void Editor::UpdateEditorPanels(float p_deltaTime)
{
	auto& profiler = m_panelManager.GetPanelAs<Panel::Profiler>("Profiler");
	
	profiler.Update(p_deltaTime);
}

void Editor::PrepareRendering(float p_deltaTime)
{
	m_context.engineUBO->SetSubData(m_context.device->GetElapsedTime(), 3 * sizeof(FMatrix4) + sizeof(FVector3));
}

void Editor::RenderViews(float p_deltaTime)
{
	auto& sceneView = m_panelManager.GetPanelAs<Panel::SceneView>("Scene View");
	auto& gameView = m_panelManager.GetPanelAs<Panel::GameView>("Game View");

	gameView.Update(p_deltaTime);
	sceneView.Update(p_deltaTime);

	gameView.Render();
	sceneView.Render();
}

void Editor::RenderEditorUI(float p_deltaTime)
{
}
