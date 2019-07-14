// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Application.h"
#include "Common/Platform/Platform.h"
#include "Common/Logger.h"
#include "Content/ContentManager.h"
#include "Core/SubSystems/SubSystemManager.h"
#include "Core/Threading/TaskManager.h"
#include "Core/Display.h"
#include "Core/Time.h"
#include "Input/InputManager.h"
#include "Physics/PhysicsManager.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/Renderer/Renderer.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/ObjectManager.h"
#include "Game/GameManager.h"
#include "Scene/SceneManager.h"

// EventProcessor is implemented per-platform
uint64_t EventProcessor(void*, uint32_t, uint64_t, uint64_t);

Application* Application::m_instance;

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

    Display::m_width = width;
    Display::m_height = height;

    RenderingManager::Resize(width, height);
}

void Application::InitializeSubSystems() const
{
    // Register subsystems
    SubSystemManager::Register<ScriptingManager>();
    SubSystemManager::Register<TaskManager>();
    SubSystemManager::Register<ObjectManager>();
    SubSystemManager::Register<ContentManager>();
    SubSystemManager::Register<Time>();
    SubSystemManager::Register<InputManager>();
    SubSystemManager::Register<RenderingManager>();
    SubSystemManager::Register<SceneManager>();
    SubSystemManager::Register<PhysicsManager>();
}

void Application::InitializeGame()
{
    m_gameManager.reset(new GameManager());
}

void Application::ShutdownGame()
{
    // Release manager's object, this will shutdown managed game instance
    m_instance->m_gameManager.reset();
}

void Application::Update()
{
    // Update input
    InputManager::GetInstance()->UpdateInput();

    // Run window events
    Platform::RunEvents();

    // Dispatch input
    InputManager::GetInstance()->DispatchInput();

    SubSystemManager::GetInstance()->Update();

    m_instance->m_gameManager->Update();

    SubSystemManager::GetInstance()->LateUpdate();
}

void Application::FixedUpdate()
{
    SubSystemManager::GetInstance()->FixedUpdate();

    m_instance->m_gameManager->FixedUpdate();
}

void Application::Render()
{
    // Render
    RenderingManager::GetInstance()->Render();

    SubSystemManager::GetInstance()->FrameDone();
}

void Application::Run()
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

    // Update display
    Display::ForceUpdate();

    // Register subsystems
    InitializeSubSystems();

    // Initialize game
    InitializeGame();

    // Run main loop
    m_mainLoop->Run();

    Logger::Log("MainLoop stopped");

    // Main loop exited.
    // When Application will get out of scope, destructor will 
    // release all resources (look Application::~Application()).
}

void Application::Shutdown()
{
    // Shutdown game
    ShutdownGame();

    // Release resources
    m_mainLoop.reset();
    m_window.reset();

    // Shutdown all subsystems and Dispose SubSystemManager (needed to cleanup the singleton)
    SubSystemManager::GetInstance()->Shutdown();
    SubSystemManager::GetInstance()->Dispose();

    // Shutdown platform
    Platform::Shutdown();

    Logger::Log("Bye");
    Logger::Shutdown();

    m_instance = nullptr;
}

void Application::Quit()
{
    ASSERT(m_instance);
    ASSERT(m_instance->m_mainLoop);

    Logger::Log("Quit...");

    m_instance->m_mainLoop->Quit();
}
