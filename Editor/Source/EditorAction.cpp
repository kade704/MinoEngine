#include <algorithm>

#include "EditorAction.h"
#include "Panel/Inspector.h"
#include "Panel/GameView.h"
#include "Panel/SceneView.h"
#include <Mino/Logger.h>
#include <Mino/SaveFileDialog.h>

EditorAction::EditorAction(Context& p_context, EditorRenderer& p_renderer, PanelManager& p_panelManager) :
    m_context(p_context),
    m_renderer(p_renderer),
    m_panelManager(p_panelManager)
{
    ServiceLocator::Provide<EditorAction>(*this);
}

Context& EditorAction::GetContext()
{
    return m_context;
}

EditorRenderer& EditorAction::GetRenderer()
{
    return m_renderer;
}

PanelManager& EditorAction::GetPanelManager()
{
    return m_panelManager;
}

void EditorAction::SelectActor(Actor& p_target)
{
    EDITOR_PANEL(Panel::Inspector, "Inspector").FocusActor(p_target);
}

void EditorAction::UnselectActor()
{
    EDITOR_PANEL(Panel::Inspector, "Inspector").Unfocus();
}

bool EditorAction::IsAnyActorSelected() const
{
    return EDITOR_PANEL(Panel::Inspector, "Inspector").GetTargetActor() != nullptr;
}

Actor& EditorAction::GetSelectedActor() const
{
    return *EDITOR_PANEL(Panel::Inspector, "Inspector").GetTargetActor();
}

Actor& EditorAction::CreateActor(Actor* p_parent, const std::string& p_name)
{
    const auto currentScene = m_context.sceneManager.GetCurrentScene();
    auto& instance = p_name.empty() ? currentScene->CreateActor() : currentScene->CreateActor(p_name);

    if (p_parent)
        instance.SetParent(*p_parent);

    MINO_LOG("Actor created");

    return instance;
}

bool EditorAction::DestroyActor(Actor& p_actor)
{
    p_actor.MarkAsDestroy();
    MINO_LOG("Actor destroyed");
    return true;
}

void EditorAction::DuplicateActor(Actor& p_toDuplicate, Actor* p_forcedParent)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLNode* actorsRoot = doc.NewElement("actors");
    p_toDuplicate.OnSerialize(doc, actorsRoot);
    auto& newActor = CreateActor();
    int64_t idToUse = newActor.GetID();
    tinyxml2::XMLElement* currentActor = actorsRoot->FirstChildElement("actor");
    newActor.OnDeserialize(doc, currentActor);

    newActor.SetID(idToUse);

    auto currentScene = m_context.sceneManager.GetCurrentScene();

    if (newActor.GetParentID() > 0)
    {
        if (auto found = currentScene->FindActorByID(newActor.GetParentID()); found)
        {
            newActor.SetParent(*found);
        }
    }

    newActor.SetName(p_toDuplicate.GetName());
    
    for (auto& child : p_toDuplicate.GetChildren())
        DuplicateActor(*child, &newActor);
}

EditorAction::EEditorMode EditorAction::GetCurrentEditorMode() const
{
    return m_editorMode;
}

void EditorAction::SetEditorMode(EEditorMode p_newEditorMode)
{
    m_editorMode = p_newEditorMode;
    EditorModeChangedEvent.Invoke(m_editorMode);
}

void EditorAction::StartPlaying()
{
    if (m_editorMode == EEditorMode::EDIT)
    {
        m_context.scriptInterpreter->RefreshAll();
        EDITOR_PANEL(Panel::Inspector, "Inspector").Refresh();

        if (m_context.scriptInterpreter->IsOk())
        {
            PlayEvent.Invoke();
            m_sceneBackup.Clear();
            tinyxml2::XMLNode* node = m_sceneBackup.NewElement("root");
            m_sceneBackup.InsertFirstChild(node);
            m_context.sceneManager.GetCurrentScene()->OnSerialize(m_sceneBackup, node);
            m_panelManager.GetPanelAs<Panel::GameView>("Game View").Focus();
            m_context.sceneManager.GetCurrentScene()->Play();
            SetEditorMode(EEditorMode::PLAY);
        }
    }
    else
    {
        SetEditorMode(EEditorMode::PLAY);
    }
}

void EditorAction::PauseGame()
{
    SetEditorMode(EEditorMode::PAUSE);
}

void EditorAction::StopPlaying()
{
    if (m_editorMode != EEditorMode::EDIT)
    {
        SetEditorMode(EEditorMode::EDIT);

        int64_t focusedActorID = -1;

        if (auto targetActor = EDITOR_PANEL(Panel::Inspector, "Inspector").GetTargetActor())
            focusedActorID = targetActor->GetID();

        m_context.sceneManager.LoadSceneFromMemory(m_sceneBackup);

        m_sceneBackup.Clear();
        EDITOR_PANEL(Panel::SceneView, "Scene View").Focus();
        if (auto actorInstance = m_context.sceneManager.GetCurrentScene()->FindActorByID(focusedActorID))
            EDITOR_PANEL(Panel::Inspector, "Inspector").FocusActor(*actorInstance);
    }
}

void EditorAction::NextFrame()
{
}

void EditorAction::RefreshScripts()
{
    m_context.scriptInterpreter->RefreshAll();
    m_panelManager.GetPanelAs<Panel::Inspector>("Inspector").Refresh();

    if (m_context.scriptInterpreter->IsOk())
        MINO_LOG("Scripts interpretation succeeded!");
}

void EditorAction::LoadEmptyScene()
{
    m_context.sceneManager.LoadEmptyScene();
    MINO_LOG("New scene created");
}

void EditorAction::LoadSceneFromDisk(const std::string& p_path)
{
    m_context.sceneManager.LoadScene(p_path);
    MINO_LOG("Scene loaded from disk: " + m_context.sceneManager.GetCurrentSceneSourcePath());
}

void EditorAction::SaveCurrentSceneTo(const std::string& p_path)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLNode* node = doc.NewElement("root");
    doc.InsertFirstChild(node);
    m_context.sceneManager.GetCurrentScene()->OnSerialize(doc, node);
    doc.SaveFile(p_path.c_str());
}

void EditorAction::SaveSceneChanges()
{
    if (IsCurrentSceneLoadedFromDisk())
    {
        SaveCurrentSceneTo(m_context.sceneManager.GetCurrentSceneSourcePath());
        MINO_LOG("Current scene saved to: " + m_context.sceneManager.GetCurrentSceneSourcePath());
    }
    else
    {
        SaveAs();
    }
}

void EditorAction::SaveAs()
{
    SaveFileDialog dialog("New Scene");
    dialog.DefineExtension("Scene", ".scene");
    dialog.Show();

    if (dialog.HasSucceeded())
    {
        SaveCurrentSceneTo(dialog.GetSelectedFilePath());
        MINO_LOG("Current scene saved to: " + dialog.GetSelectedFilePath());
    }
}

void EditorAction::DelayAction(std::function<void()> p_action, uint32_t p_frames)
{
    m_delayedActions.emplace_back(p_frames + 1, p_action);
}

void EditorAction::ExecuteDelayedActions()
{
    std::for_each(m_delayedActions.begin(), m_delayedActions.end(), [](std::pair<uint32_t, std::function<void()>>& p_element)
    {
        --p_element.first;

        if (p_element.first == 0)
            p_element.second();
    });

    m_delayedActions.erase(std::remove_if(m_delayedActions.begin(), m_delayedActions.end(), [](std::pair<uint32_t, std::function<void()>>& p_element)
    {
        return p_element.first == 0;
    }), m_delayedActions.end());
}

bool EditorAction::IsCurrentSceneLoadedFromDisk() const
{
    return m_context.sceneManager.IsCurrentSceneLoadedFromDisk();
}