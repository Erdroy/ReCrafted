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
#include "Scripting/ScriptingEngine.h"
#include "Platform/Platform.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/Rendering.h"
#include "Graphics/UI/UI.h"
#include "Game/Universe.h"

EngineMain* EngineMain::m_instance;

void EngineMain::registerComponents() const
{
    // initialize the rest of the engine components
    m_componentManager->registerComponent(new Rendering());
    m_componentManager->registerComponent(new Application());
    m_componentManager->registerComponent(new Profiler());
    m_componentManager->registerComponent(new Time());
    m_componentManager->registerComponent(new Input());
    m_componentManager->registerComponent(new PhysicsManager());
    m_componentManager->registerComponent(new EntityPool());
    m_componentManager->registerComponent(new Universe());
    m_componentManager->registerComponent(new UI());
}

void EngineMain::createMainWindow()
{
    m_mainWindow = std::make_shared<ApplicationWindow>();
    m_mainWindow->create();
    m_mainWindow->setOnResized(MakeDelegate(EngineMain::onWindowResized));

    uint width;
    uint height;

    // get game window size
    Platform::getCurrentWindowSize(&width, &height); // TODO: get width/height for ApplicationWindow

    Display::set_Width(width);
    Display::set_Height(height);
}

void EngineMain::onSimulate()
{
    // update fixed time
    Time::m_instance->m_fixedTime = static_cast<float>(static_cast<double>(Time::m_instance->m_fixedTime) + Time::m_instance->m_fixedDeltaTime);

    // simulate
    Universe::getInstance()->simulate();
    Application::getInstance()->simulate();
}

void EngineMain::onUpdate()
{
    Profiler::beginProfile("Frame");

    // update time
    // TODO: Time::getInstance()->update();
    cvar currentTime = Platform::getMiliseconds();
    Time::m_instance->m_deltaTime = (currentTime - m_lastUpdateTime) / 1000.0;
    Time::m_instance->m_time = float(currentTime / 1000.0);
    m_lastUpdateTime = currentTime;

    // update input
    Input::getInstance()->update();

    // clear keyboard buffer
    KeyboardBuffer::clear();

    // run platform events
    Platform::runEvents();

    // update
    Profiler::update();
    EntityPool::getInstance()->update();
    Rendering::getInstance()->update();
    Universe::getInstance()->update();
    Application::getInstance()->update();
}

void EngineMain::onRender()
{
    Profiler::beginProfile("Render");
    {
        Rendering::getInstance()->render();
    }
    Profiler::endProfile();
    
    // end 'Frame' profile
    Profiler::endProfile();
    Profiler::endFrame();
}

void EngineMain::onWindowResized()
{
    // resize now
    Rendering::getInstance()->resize(m_mainWindow->get_width(), m_mainWindow->get_height());
}

void EngineMain::initialize()
{
    // initialize component manager
    m_componentManager = EngineComponentManager::getInstance();

    // initialize basic pre-log components
    m_componentManager->registerComponent(new ScriptingEngine());
    m_componentManager->registerComponent(new Logger());

    // Say something, as now we have scripting and logger initialized, 
    // so we can scream around.
    Logger::log("ReCrafted startup");

    // initialize platform
    Platform::initialize();

    // create main window
    createMainWindow();

    // register all needed components
    registerComponents();
}

void EngineMain::run()
{
    Logger::log("ReCrafted load");

    // call load callback
    m_componentManager->onLoad();

    Logger::log("ReCrafted run");

    // create update loop
    m_updateLoop = std::make_shared<UpdateLoop>();

    // set loop callbacks
    m_updateLoop->setSimulateCallback(MakeDelegate(EngineMain::onSimulate));
    m_updateLoop->setUpdateCallback(MakeDelegate(EngineMain::onUpdate));
    m_updateLoop->setRenderCallback(MakeDelegate(EngineMain::onRender));

    // start loop
    m_updateLoop->start();
}

void EngineMain::shutdown()
{
    Logger::log("ReCrafted shutdown");
    Logger::logInfo("Shutting down...");

    // dispose window (using Ptr<> - no need to delete)
    SafeDispose(m_mainWindow);

    // dispose component manager at the very end
    SafeDispose(m_componentManager);
}

void EngineMain::quit()
{
    // stop update loop
    m_updateLoop->stop();
}
