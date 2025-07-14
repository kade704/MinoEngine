#pragma once

#include "../Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void LoadEmptyScene();
	bool LoadScene(const std::string& p_path, bool p_absolute = false);
	bool LoadSceneFromMemory(tinyxml2::XMLDocument& p_doc);

	void UnloadCurrentScene();

	bool HasCurrentScene() const;
	Scene* GetCurrentScene();

	std::string GetCurrentSceneSourcePath() const;

	bool IsCurrentSceneLoadedFromDisk() const;

public:
	Event<> SceneLoadEvent;
	Event<> SceneUnloadEvent;

private:	
	Scene* m_currentScene = nullptr;
	bool m_currentSceneLoadedFromPath = false;
	std::string m_currentSceneSourcePath = "";
};

