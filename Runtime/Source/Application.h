#pragma once
#include "Context.h"
#include "Runtime.h"

class Application
{
public:
    Application();
    ~Application();

    void Run();

    bool IsRunning() const;

private:
    Context m_context;
    Runtime m_runtime;
};