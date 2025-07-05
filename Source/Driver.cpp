#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Logger.h"
#include "Driver.h"

Driver::Driver()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw "Failed to initialize OpenGL loader!";
    }

    mIsActive = true;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
}

Driver::~Driver()
{
}

bool Driver::IsActive() const
{
    return mIsActive;
}
