#include <GLFW/glfw3.h>
#include <iostream>

#include "Device.h"

Device::Device(DeviceSettings& deviceSettings)
{
    int initCode = glfwInit();
    if (initCode == GLFW_FALSE)
    {
        throw "Failed to initialize GLFW";
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, deviceSettings.contextMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, deviceSettings.contextMinorVersion);
    glfwWindowHint(GLFW_SAMPLES, deviceSettings.samples);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mIsAlive = true;
}

Device::~Device()
{
    if (mIsAlive)
    {
        glfwTerminate();
    }
}

void Device::PollEvents() const
{
    glfwPollEvents();
}

float Device::GetElapsedTime() const
{
    return static_cast<float>(glfwGetTime());
}
