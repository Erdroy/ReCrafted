// ReCrafted (c) 2016-2018 Always Too Late

#include "EngineMain.h"
#include "EngineComponentManager.h"

#include "Common/Time.h"
#include "Common/Display.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Input/Input.h"
#include "Common/Input/KeyboardBuffer.h"
#include "Common/EntityPool.h"
#include "Core/Application.h"
#include "Core/Logger.h"
#include "Core/UpdateLoop.h"
#include "Game/Universe.h"
#include "Graphics/Graphics.h"
#include "Graphics/DebugDraw.h"
#include "Graphics/UI/UI.h"
#include "Graphics/WebUI/WebUI.h"
#include "Platform/Platform.h"
#include "Physics/PhysicsManager.h"
#include "Scene/SceneManager.h"
#include "Scripting/ScriptingEngine.h"
#include "TaskManager.h"

EngineMain* EngineMain::m_instance;

void EngineMain::RegisterComponents() const
{
    // initialize the rest of the engine components
    m_componentManager->RegisterComponent(TaskManager::GetInstance());
    m_componentManager->RegisterComponent(Graphics::GetInstance());
    m_componentManager->RegisterComponent(Application::GetInstance());
    m_componentManager->RegisterComponent(Profiler::GetInstance());
    m_componentManager->RegisterComponent(Time::GetInstance());
    m_componentManager->RegisterComponent(Input::GetInstance());
    m_componentManager->RegisterComponent(PhysicsManager::GetInstance());
    m_componentManager->RegisterComponent(EntityPool::GetInstance());
    m_componentManager->RegisterComponent(SceneManager::GetInstance());
    m_componentManager->RegisterComponent(Universe::GetInstance());
    m_componentManager->RegisterComponent(DebugDraw::GetInstance());
    m_componentManager->RegisterComponent(UI::GetInstance());
    m_componentManager->RegisterComponent(WebUI::GetInstance());
}

void EngineMain::CreateMainWindow()
{
    m_mainWindow.reset(new ApplicationWindow());
    m_mainWindow->Create();
    m_mainWindow->SetOnResized(MakeDelegate(EngineMain::OnWindowResized));

    // Update size
    m_mainWindow->UpdateSizeNow();

    Display::SetWidth(m_mainWindow->GetWidth());
    Display::SetHeight(m_mainWindow->GetHeight());
}

void EngineMain::OnSimulate()
{
    // Update fixed time
    Time::m_instance->m_fixedTime = static_cast<float>(static_cast<double>(Time::m_instance->m_fixedTime) + Time::
        m_instance->m_fixedDeltaTime);

    // Simulate
    Universe::GetInstance()->Simulate();
    Application::GetInstance()->Simulate();
}

void EngineMain::OnUpdate()
{
    // Update time
    // TODO: Time::GetInstance()->Update();
    cvar currentTime = Platform::GetMiliseconds();
    Time::m_instance->m_deltaTime = (currentTime - m_lastUpdateTime) / 1000.0;
    Time::m_instance->m_time = float(currentTime / 1000.0);
    m_lastUpdateTime = currentTime;

    // Update input
    Input::GetInstance()->UpdateInput();

    // clear keyboard buffer
    KeyboardBuffer::Clear();

    // run platform events
    Platform::RunEvents();

    // Update all components
    m_componentManager->Update();
}

void EngineMain::OnRender()
{
    Graphics::GetInstance()->Render();
}

void EngineMain::OnWindowResized()
{
    // resize now
    Graphics::GetInstance()->Resize(m_mainWindow->GetWidth(), m_mainWindow->GetHeight());
}

void EngineMain::Initialize()
{
    Platform::SetThreadName("Main Thread");

    // create Update loop
    m_updateLoop.reset(new UpdateLoop());

    // initialize component manager
    m_componentManager = EngineComponentManager::GetInstance();

    // initialize basic pre-log components
    m_componentManager->RegisterComponent(ScriptingEngine::GetInstance());
    m_componentManager->RegisterComponent(Logger::GetInstance());

    // Say something, as now we have scripting and logger initialized, 
    // so we can scream around.
    Logger::Log("ReCrafted startup");

    // initialize platform
    Platform::Initialize();

    // create main window
    CreateMainWindow();

    // register all needed components
    RegisterComponents();
}

void EngineMain::Run()
{
    Logger::Log("ReCrafted load");

    // call load callback
    m_componentManager->OnLoad();

    Logger::Log("ReCrafted run");

    // set loop callbacks
    m_updateLoop->SetSimulateCallback(MakeDelegate(EngineMain::OnSimulate));
    m_updateLoop->SetUpdateCallback(MakeDelegate(EngineMain::OnUpdate));
    m_updateLoop->SetRenderCallback(MakeDelegate(EngineMain::OnRender));

    // start loop
    m_updateLoop->Start();
}

void EngineMain::Shutdown()
{
    Logger::Log("ReCrafted Shutdown");
    Logger::LogInfo("Shutting down...");

    // Dispose window (using Ref<> - no need to delete)
    SafeDispose(m_mainWindow);

    // Dispose component manager at the very end
    SafeDispose(m_componentManager);
}

void EngineMain::Quit()
{
    // stop Update loop
    m_updateLoop->Stop();
}
