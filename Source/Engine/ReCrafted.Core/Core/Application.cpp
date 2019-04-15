// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Application.h"
#include "Common/Platform/Platform.h"
#include "Core/SubSystems/SubSystemManager.h"
#include "Core/Time.h"
#include "Core/Input/InputManager.h"

// EventProcessor is implemented per-platform
uint64_t EventProcessor(void*, uint32_t, uint64_t, uint64_t);

Application* Application::m_instance;

Application::Application()
{
    m_instance = this;

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

    // Register subsystems
    RegisterSubSystems();
}

Application::~Application()
{
    // Release resources
    m_mainLoop.reset();
    m_window.reset();

    // Shutdown all subsystems and Dispose SubSystemManager (needed to cleanup the singleton)
    SubSystemManager::GetInstance()->Shutdown();
    SubSystemManager::GetInstance()->Dispose();

    // Shutdown platform
    Platform::Shutdown();

    // TODO: Flush and release logger

    m_instance = nullptr;
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
    SubSystemManager::Register<InputManager>();
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
}

void Application::Run()
{
    // Run main loop
    m_mainLoop->Run();

    // Main loop exited.
    // When Application will get out of scope, destructor will 
    // release all resources (look Application::~Application()).
}

void Application::Quit()
{
    ASSERT(m_instance);
    ASSERT(m_instance->m_mainLoop);
    m_instance->m_mainLoop->Quit();
}
