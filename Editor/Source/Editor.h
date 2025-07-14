#pragma once

#include <Mino/PanelManager.h>
#include <Mino/Canvas.h>
#include "Context.h"
#include "EditorRenderer.h"
#include "EditorAction.h"

class Editor
{
public:
	Editor(Context& p_context);
	~Editor();

	void SetupUI();

	void PreUpdate();
	void Update(float p_deltaTime);
	void PostUpdate();

	void UpdateCurrentEditorMode(float p_deltaTime);
	void UpdatePlayMode(float p_deltaTime);
	void UpdateEditMode(float p_deltaTime);
	void UpdateEditorPanels(float p_deltaTime);
	void PrepareRendering(float p_deltaTime);
	void RenderViews(float p_deltaTime);
	void RenderEditorUI(float p_deltaTime);

private:
	Context& m_context;
	Canvas m_canvas;
	PanelManager m_panelManager;
	EditorRenderer m_editorRenderer;
	EditorAction m_editorAction;
};

