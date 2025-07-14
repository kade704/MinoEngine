#pragma once
#include "Context.h"
#include "Editor.h"

class Application
{
public:
	Application();
	~Application();

	void Run();

	bool IsRunning();

private:
	Context m_context;
	Editor m_editor;
};

