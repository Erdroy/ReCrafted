// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Application.h"
#include "Common/Platform/Platform.h"
#include "Core/SubSystems/SubSystemManager.h"
#include "Core/Time.h"

Application::Application()
{
    // TODO: Create logger

    // Initialize platform
    Platform::Initialize();

    // Initialize SubSystemManager instance
    SubSystemManager::GetInstance();

    m_mainLoop.reset(new MainLoop());
}

Application::~Application()
{
    // Release main loop
    m_mainLoop.reset();

    // Shutdown all subsystems
    SubSystemManager::GetInstance()->Shutdown();

    // Shutdown platform
    Platform::Initialize();

    // TODO: Flush and release logger
}

void Application::RegisterSubSystems() const
{
    // Register subsystems
    SubSystemManager::Register<Time>();
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
