// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Application.h"
#include "Common/Platform/Platform.h"
#include "Common/Logger.h"
#include "SubSystems/SubSystemManager.h"
#include "Core/Time.h"
#include "Input/InputManager.h"
#include "Actors/ActorPoolManager.h"
#include "Renderer/Renderer.h"

// EventProcessor is implemented per-platform
uint64_t EventProcessor(void*, uint32_t, uint64_t, uint64_t);

Application* Application::m_instance;

Application::Application()
{
    m_instance = this;

    // Initialize platform
    Platform::Initialize(&EventProcessor);
    Platform::SetThreadName("MainThread");

    // Initialize logger
    Logger::Initialize();

    // Create game window
    CreateGameWindow();

    // Initialize SubSystemManager instance
    SubSystemManager::GetInstance();

    m_mainLoop.reset(new MainLoop());

    // Set main loop callbacks
    m_mainLoop->SetUpdateCallback(Action<void>::New<Application, &Application::Update>(this));
    m_mainLoop->SetFixedUpdateCallback(Action<void>::New<Application, &Application::FixedUpdate>(this));
    m_mainLoop->SetRenderCallback(Action<void>::New<Application, &Application::Render>(this));

    // Register subsystems
    InitializeSubSystems();

    // Initialize renderer
    InitializeRenderer();
}

Application::~Application()
{
    // Shutdown renderer
    Renderer::Shutdown();

    // Release resources
    m_mainLoop.reset();
    m_window.reset();

    // Shutdown all subsystems and Dispose SubSystemManager (needed to cleanup the singleton)
    SubSystemManager::GetInstance()->Shutdown();
    SubSystemManager::GetInstance()->Dispose();

    // Shutdown platform
    Platform::Shutdown();

    Logger::Shutdown();

    m_instance = nullptr;
}

void Application::CreateGameWindow()
{
    m_window.reset(new ApplicationWindow());
    m_window->SetOnResized(Action<void>::New<Application, &Application::OnWindowResized>(this));
}

void Application::OnWindowResized()
{
    uint width;
    uint height;
    Platform::GetWindowSize(Platform::GetCurrentWindow(), &width, &height);

    // TODO: Resize!
    Renderer::ResizeWindow(m_windowHandle, width, height);
}

void Application::InitializeSubSystems() const
{
    // Register subsystems
    SubSystemManager::Register<Time>();
    SubSystemManager::Register<InputManager>();
    SubSystemManager::Register<ActorPoolManager>();
}

void Application::InitializeRenderer()
{
    Logger::Log("Creating renderer with Direct3D11 API");

    Renderer::Initialize(
        Renderer::RendererAPI::DirectX11,
        Renderer::Settings::Debug,
        Renderer::RenderFlags::_enum(Renderer::RenderFlags::Default));

    Renderer::SetFlag(Renderer::RenderFlags::VSync, false);

    // Create Output
    m_windowHandle = Renderer::CreateWindowHandle(Platform::GetCurrentWindow());
    m_frameBufferHandle = Renderer::GetWindowRenderBuffer(m_windowHandle);
}

void Application::InitializeGraphics()
{
}

void Application::Update()
{
    // Update input
    InputManager::GetInstance()->UpdateInput();

    // Run window events
    Platform::RunEvents();

    // Dispatch input
    InputManager::GetInstance()->DispatchInput();

    // TODO: Update rest of the world
}

void Application::FixedUpdate()
{

}

void Application::Render()
{
    Renderer::ClearRenderBuffer(m_frameBufferHandle, Renderer::Color(0.15f, 0.15f, 0.15f, 1.0f));

    Renderer::Frame();
}

void Application::Run()
{
    // Run main loop
    m_mainLoop->Run();

    Logger::Log("MainLoop stopped");

    // Main loop exited.
    // When Application will get out of scope, destructor will 
    // release all resources (look Application::~Application()).
}

void Application::Quit()
{
    ASSERT(m_instance);
    ASSERT(m_instance->m_mainLoop);

    Logger::Log("Quit...");

    m_instance->m_mainLoop->Quit();
}
