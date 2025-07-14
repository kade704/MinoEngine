#pragma once

#include <Mino/ServiceLocator.h>
#include <Mino/PanelManager.h>

#include "Context.h"
#include "EditorRenderer.h"

#define EDITOR_EXEC(action)					ServiceLocator::Get<EditorAction>().action
#define EDITOR_BIND(method, ...)			std::bind(&EditorAction::method, &ServiceLocator::Get<EditorAction>(), ##__VA_ARGS__)
#define EDITOR_EVENT(target)				ServiceLocator::Get<EditorAction>().target
#define EDITOR_CONTEXT(instance)			ServiceLocator::Get<EditorAction>().GetContext().instance
#define EDITOR_RENDERER()					ServiceLocator::Get<EditorAction>().GetRenderer()
#define EDITOR_PANEL(type, id)				ServiceLocator::Get<EditorAction>().GetPanelManager().GetPanelAs<type>(id)

class EditorAction
{
public:
	enum class EEditorMode { EDIT, PLAY, PAUSE, FRAME_BY_FRAME };

	EditorAction(Context& p_context, EditorRenderer& p_renderer, PanelManager& p_panelManager);

	Context& GetContext();
	EditorRenderer& GetRenderer();
	PanelManager& GetPanelManager();

	void SelectActor(Actor& p_target);
	void UnselectActor();
	bool IsAnyActorSelected() const;
	Actor& GetSelectedActor() const;
	Actor& CreateActor(Actor* p_parent, const std::string& p_name);
	bool DestroyActor(Actor& p_actor);

	EEditorMode GetCurrentEditorMode() const;
	void SetEditorMode(EEditorMode p_newEditorMode);

	void StartPlaying();
	void PauseGame();
	void StopPlaying();
	void NextFrame();
	void RefreshScripts();

	void LoadEmptyScene();
	void LoadSceneFromDisk(const std::string& p_path);

	void SaveCurrentSceneTo(const std::string& p_path);
	void SaveSceneChanges();
	void SaveAs();

	void DelayAction(std::function<void()> p_action, uint32_t p_frames = 1);
	void ExecuteDelayedActions();

	bool IsCurrentSceneLoadedFromDisk() const;

private:
	Context& m_context;
	PanelManager& m_panelManager;
	EditorRenderer& m_renderer;
	EEditorMode m_editorMode = EEditorMode::EDIT;
	std::vector<std::pair<uint32_t, std::function<void()>>> m_delayedActions;

public:
	Event<Actor&> ActorSelectedEvent;
	Event<Actor&> ActorUnselectedEvent;
};

