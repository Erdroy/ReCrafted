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
#include "Content/ContentManager.h"
#include "Game/Universe.h"
#include "Graphics/Graphics.h"
#include "Graphics/DebugDraw.h"
#include "UI/UI.h"
#include "WebUI/WebUI.h"
#include "Physics/PhysicsManager.h"
#include "Platform/Platform.h"
#include "Scene/SceneManager.h"
#include "Scripting/ScriptingEngine.h"
#include "TaskManager.h"
#include "Voxels/VoxelMaterialManager.h"
#include "Audio/AudioManager.h"

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
    m_componentManager->RegisterComponent(AudioManager::GetInstance());
    m_componentManager->RegisterComponent(EntityPool::GetInstance());
    m_componentManager->RegisterComponent(VoxelMaterialManager::GetInstance());
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
    m_mainWindow->SetOnResized(Action<void>::New<EngineMain, &EngineMain::OnWindowResized>(this));

    // Update size
    m_mainWindow->UpdateSizeNow();

    Display::SetWidth(m_mainWindow->GetWidth());
    Display::SetHeight(m_mainWindow->GetHeight());
}

void EngineMain::OnSimulate()
{
    Profiler::BeginProfile("Simulate");
    {
        // Update fixed time
        Time::m_instance->m_fixedTime = static_cast<float>(static_cast<double>(Time::m_instance->m_fixedTime) + Time::
            m_instance->m_fixedDeltaTime);

        // Simulate
        PhysicsManager::GetInstance()->Simulate();
        Universe::GetInstance()->Simulate();
        Application::GetInstance()->Simulate();
    }
    Profiler::EndProfile();
}

void EngineMain::OnUpdate()
{
    // Update time
    Time::GetInstance()->OnFrame();

    Profiler::BeginProfile("Process Input");
    {
        // Update input
        Input::GetInstance()->UpdateInput();
    }
    Profiler::EndProfile();

    // clear keyboard buffer
    KeyboardBuffer::Clear();

    // run platform events
    Platform::RunEvents();

    Profiler::BeginProfile("Update");
    {
        // Update all components
        m_componentManager->Update();
    }
    Profiler::EndProfile();
}

void EngineMain::OnRender()
{
    ContentManager::GetInstance()->PreFrame();

    Graphics::GetInstance()->Render();
}

void EngineMain::OnWindowResized()
{
    // resize now
    Graphics::GetInstance()->Resize(m_mainWindow->GetWidth(), m_mainWindow->GetHeight());

    // Invoke resize event
    OnMainWindowResize.Invoke(m_mainWindow->GetWidth(), m_mainWindow->GetHeight());
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
    m_componentManager->RegisterComponent(ContentManager::GetInstance());

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
    m_updateLoop->SetSimulateCallback(Action<void>::New<EngineMain, &EngineMain::OnSimulate>(this));
    m_updateLoop->SetUpdateCallback(Action<void>::New<EngineMain, &EngineMain::OnUpdate>(this));
    m_updateLoop->SetRenderCallback(Action<void>::New<EngineMain, &EngineMain::OnRender>(this));

    // start loop
    m_updateLoop->Start();
}

void EngineMain::Shutdown()
{
    Logger::Log("ReCrafted Shutdown");
    Logger::LogInfo("Shutting down...");

    // Dispose window (using Ref<> - no need to delete)
    SafeDispose(m_mainWindow);

    // Release content manager
    m_componentManager->UnregisterComponent(ContentManager::GetInstance());

    // Dispose component manager at the very end
    SafeDispose(m_componentManager);
}

void EngineMain::Quit()
{
    // stop Update loop
    m_updateLoop->Stop();
}
