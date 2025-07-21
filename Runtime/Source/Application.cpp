#include "Application.h"
#include <Mino/Clock.h>

Application::Application() :
	m_runtime(m_context)
{

}

Application::~Application()
{
}

void Application::Run()
{
	Clock clock;

	while (IsRunning())
	{
		m_runtime.PreUpdate();
		m_runtime.Update(clock.GetDeltaTime());
		m_runtime.PostUpdate();
		clock.Update();
	}
}

bool Application::IsRunning() const
{
	return !m_context.window->ShouldClose();
}
