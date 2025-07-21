#include "Context.h"

#include <Mino/WindowSettings.h>
#include <Mino/DeviceSettings.h>
#include <Mino/ServiceLocator.h>
#include <Mino/Logger.h>

Context::Context()
{
    DeviceSettings deviceSettings;
	deviceSettings.contextMajorVersion = 4;
	deviceSettings.contextMinorVersion = 3;
	deviceSettings.samples = 4;
	device = std::make_unique<Device>(deviceSettings);

	WindowSettings windowSettings;
	windowSettings.title = "Mino Game";
	windowSettings.width = 1200;
	windowSettings.height = 900;
	window = std::make_unique<Window>(windowSettings);
	window->MakeContextCurrent();

    inputManager = std::make_unique<InputManager>(*window);

    driver = std::make_unique<Driver>();
    renderer = std::make_unique<Renderer>();

    uiManager = std::make_unique<UIManager>(window->GetGlfwWindow());

    ServiceLocator::Provide<ModelManager>(modelManager);
	ServiceLocator::Provide<TextureManager>(textureManager);
	ServiceLocator::Provide<MaterialManager>(materialManager);
	ServiceLocator::Provide<ShaderManager>(shaderManager);
}

Context::~Context()
{
}