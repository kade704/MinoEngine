#include "SceneManager.h"
#include "../Logger.h"

SceneManager::SceneManager()
{
    LoadEmptyScene();
}

SceneManager::~SceneManager()
{
}

void SceneManager::LoadEmptyScene()
{
    UnloadCurrentScene();

    m_currentScene = new Scene();

    SceneLoadEvent.Invoke();
}

bool SceneManager::LoadScene(const std::string& p_path, bool p_absolute)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(p_path.c_str());

    if (LoadSceneFromMemory(doc))
    {
        m_currentSceneSourcePath = p_path;
        m_currentSceneLoadedFromPath = true;
        return true;
    }

    return false;
}

bool SceneManager::LoadSceneFromMemory(tinyxml2::XMLDocument& p_doc)
{
    if (!p_doc.Error())
    {
        tinyxml2::XMLNode* root = p_doc.FirstChild();
        if (root)
        {
            LoadEmptyScene();
            m_currentScene->OnDeserialize(p_doc, root);
            return true;
        }
    }

    MINO_ERROR("Scene loading failed. The scene you are trying to load was not found or corrupted");
    return false;
}

void SceneManager::UnloadCurrentScene()
{
    if (m_currentScene)
    {
        delete m_currentScene;
        m_currentScene = nullptr;
        SceneUnloadEvent.Invoke();
    }
}

bool SceneManager::HasCurrentScene() const
{
    return m_currentScene;
}

Scene* SceneManager::GetCurrentScene()
{
    return m_currentScene;
}

std::string SceneManager::GetCurrentSceneSourcePath() const
{
    return m_currentSceneSourcePath;
}

bool SceneManager::IsCurrentSceneLoadedFromDisk() const
{
    return m_currentSceneLoadedFromPath;
}
