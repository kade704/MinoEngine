#include "Application.h"
#include <Mino/Clock.h>

Application::Application() :
	m_context(),
	m_editor(m_context)
{
}

Application::~Application()
{
}

void Application::Run()
{
	Clock clock;
	clock.Initialize();

	while (IsRunning())
	{
		m_editor.PreUpdate();
		m_editor.Update(clock.GetDeltaTime());
		m_editor.PostUpdate();

		clock.Update();
	}
}

bool Application::IsRunning()
{
	return !m_context.window->ShouldClose();
}
