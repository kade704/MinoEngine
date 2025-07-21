#pragma once

#include "Context.h"

class Runtime
{
public:
    Runtime(Context& context);
    ~Runtime();

    void PreUpdate();
    void Update(float deltaTime);
    void PostUpdate();

private:
    Context& m_context;
};