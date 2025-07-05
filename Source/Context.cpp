

#include "Context.h"
#include "WindowSettings.h"
#include "DeviceSettings.h"
#include "ServiceLocator.h"
#include "Logger.h"

Context::Context()
{
	DeviceSettings deviceSettings;
	deviceSettings.contextMajorVersion = 4;
	deviceSettings.contextMinorVersion = 3;
	deviceSettings.samples = 4;
	device = std::make_unique<Device>(deviceSettings);

	WindowSettings windowSettings;
	windowSettings.title = "Mino Engine";
	windowSettings.width = 1200;
	windowSettings.height = 900;
	window = std::make_unique<Window>(windowSettings);
	window->MakeContextCurrent();

	inputManager = std::make_unique<InputManager>(*window);

	driver = std::make_unique<Driver>();
	renderer = std::make_unique<Renderer>();

	uiManager = std::make_unique<UIManager>(window->GetGlfwWindow());
	profiler = std::make_unique<Analytics::Profiler>();

	ServiceLocator::Provide<InputManager>(*inputManager);
	ServiceLocator::Provide<Window>(*window);
	ServiceLocator::Provide<Renderer>(*renderer);

	scriptInterpreter = std::make_unique<ScriptInterpreter>();

	engineUBO = std::make_unique<Buffer::UniformBuffer>
	(
		sizeof(FMatrix4) +
		sizeof(FMatrix4) +
		sizeof(FMatrix4) +
		sizeof(FVector3),
		0, 0
	);

	lightSSBO = std::make_unique<Buffer::ShaderStorageBuffer>();
	lightSSBO->Bind(0);

	editorResource = std::make_unique<EditorResource>();

	ServiceLocator::Provide<ModelManager>(modelManager);
	ServiceLocator::Provide<TextureManager>(textureManager);
	ServiceLocator::Provide<MaterialManager>(materialManager);
	ServiceLocator::Provide<ShaderManager>(shaderManager);
}

Context::~Context()
{
}
