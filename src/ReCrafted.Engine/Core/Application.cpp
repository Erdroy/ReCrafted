// ReCrafted (c) 2016-2018 Always Too Late

#include "Application.h"
#include "Scripting/Assembly.h"
#include "Scripting/Object.h"
#include "Scripting/Method.h"

void Application::onInit()
{
    // create gamemain instance
    m_gamemain = Object::createInstance<Object>("ReCrafted.Game", "GameMain", Assembly::Game, false);

    // find methods
    m_init_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::Initialize");
    m_simulate_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::Simulate");
    m_update_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::Update");
    m_render_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::Render");
    m_renderui_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::RenderUI");
    m_shutdown_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::Shutdown");
}

void Application::onShutdown()
{
    m_shutdown_method->invoke();
}

void Application::onLoad()
{
    m_init_method->invoke();
}

void Application::simulate()
{
    m_simulate_method->invoke();
}

void Application::update()
{
    m_update_method->invoke();
}

void Application::render()
{
    m_render_method->invoke();
}

void Application::renderUI()
{
    m_renderui_method->invoke();
}

Application* Application::getInstance()
{
    return m_instance;
}
