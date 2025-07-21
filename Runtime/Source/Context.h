#pragma once

#include <Mino/Device.h>
#include <Mino/Window.h>
#include <Mino/InputManager.h>
#include <Mino/Driver.h>
#include <Mino/Renderer.h>
#include <Mino/UIManager.h>
#include <Mino/Manager/SceneManager.h>
#include <Mino/Manager/ModelManager.h>
#include <Mino/Manager/TextureManager.h>
#include <Mino/Manager/MaterialManager.h>
#include <Mino/Manager/ShaderManager.h>

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

    SceneManager sceneManager;
    ModelManager modelManager;
    TextureManager textureManager;
    MaterialManager materialManager;
    ShaderManager shaderManager;
};