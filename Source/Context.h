#pragma once

#include <memory>

#include <Device.h>
#include <Window.h>
#include <InputManager.h>
#include <Driver.h>
#include <Renderer.h>
#include <UIManager.h>
#include <Profiler.h>
#include <EditorResource.h>
#include <ScriptInterpreter.h>
#include <Buffer/UniformBuffer.h>
#include <Buffer/ShaderStorageBuffer.h>
#include <Manager/SceneManager.h>
#include <Manager/ModelManager.h>
#include <Manager/TextureManager.h>
#include <Manager/MaterialManager.h>
#include <Manager/ShaderManager.h>

class Context
{
public:
	Context();
	~Context();

public:
	std::unique_ptr<Device> device;
	std::unique_ptr<Window> window;
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<Driver> driver;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<UIManager> uiManager;
	std::unique_ptr<Analytics::Profiler> profiler;
	std::unique_ptr<Buffer::UniformBuffer> engineUBO;
	std::unique_ptr<Buffer::ShaderStorageBuffer> lightSSBO;
	std::unique_ptr<EditorResource> editorResource;
	std::unique_ptr<ScriptInterpreter> scriptInterpreter;

	SceneManager sceneManager;
	ModelManager modelManager;
	TextureManager textureManager;
	MaterialManager materialManager;
	ShaderManager shaderManager;
};

