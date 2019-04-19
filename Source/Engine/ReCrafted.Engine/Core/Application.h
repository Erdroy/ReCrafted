// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "Core/ApplicationBase.h"
#include "Core/MainLoop.h"
#include "Core/ApplicationWindow.h"
#include "Renderer/Renderer.h"

class Application final : public ApplicationBase
{
    DELETE_COPY_MOVE(Application)

private:
    static Application* m_instance;

    RefPtr<MainLoop> m_mainLoop = nullptr;
    RefPtr<ApplicationWindow> m_window = nullptr;
    Renderer::WindowHandle m_windowHandle = {};
    Renderer::RenderBufferHandle m_frameBufferHandle = {};

public:
    Application();
    ~Application();

private:
    void CreateGameWindow();
    void OnWindowResized();

    void InitializeSubSystems() const;
    void InitializeRenderer();
    void InitializeGraphics();

    void Update();
    void FixedUpdate();
    void Render();

public:
    void Run() override;

public:
    static void Quit();
};
