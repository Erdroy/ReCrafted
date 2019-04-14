// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Application.h"
#include "Common/Platform/Platform.h"
#include "Core/SubSystems/SubSystemManager.h"
#include "Core/Time.h"

// EventProcessor is implemented per-platform
uint64_t EventProcessor(void*, uint32_t, uint64_t, uint64_t);

Application::Application()
{
    // TODO: Create logger

    // Initialize platform
    Platform::Initialize(&EventProcessor);

    // Create game window
    CreateGameWindow();

    // Initialize SubSystemManager instance
    SubSystemManager::GetInstance();

    m_mainLoop.reset(new MainLoop());

    // Set main loop callbacks
    m_mainLoop->SetUpdateCallback(Action<void>::New<Application, &Application::Update>(this));
    m_mainLoop->SetFixedUpdateCallback(Action<void>::New<Application, &Application::FixedUpdate>(this));
    m_mainLoop->SetRenderCallback(Action<void>::New<Application, &Application::Render>(this));
}

Application::~Application()
{
    // Release main loop
    m_mainLoop.reset();

    // Shutdown all subsystems
    SubSystemManager::GetInstance()->Shutdown();

    // Shutdown platform
    Platform::Shutdown();

    // TODO: Flush and release logger
}

void Application::CreateGameWindow()
{
    m_window.reset(new ApplicationWindow());
    m_window->SetOnResized(Action<void>::New<Application, &Application::OnWindowResized>(this));
    
}

void Application::OnWindowResized()
{
    // TODO: Resize!
}

void Application::RegisterSubSystems() const
{
    // Register subsystems
    SubSystemManager::Register<Time>();
}


void Application::Update()
{
    // TODO: Update input

    // Run window events
    Platform::RunEvents();

}

void Application::FixedUpdate()
{
}

void Application::Render()
{
}

void Application::Run()
{
    // Register subsystems
    RegisterSubSystems();

    // Run main loop
    m_mainLoop->Run();

    // Main loop exited.
    // When Application will get out of scope, destructor will 
    // release all resources (look Application::~Application()).
}
