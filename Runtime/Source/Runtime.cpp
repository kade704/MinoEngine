#include "Runtime.h"

Runtime::Runtime(Context& context) :
    m_context(context)
{
    m_context.sceneManager.LoadScene("Contents/boxes.scene");
	m_context.sceneManager.GetCurrentScene()->Play();
}

Runtime::~Runtime()
{
}

void Runtime::PreUpdate()
{
    m_context.device->PollEvents();
}

void Runtime::Update(float deltaTime)
{
    if (auto currentScene = m_context.sceneManager.GetCurrentScene())
	{
        currentScene->Update(deltaTime);
    }
}

void Runtime::PostUpdate()
{
    m_context.window->SwapBuffers();
	m_context.inputManager->ClearEvents();
}