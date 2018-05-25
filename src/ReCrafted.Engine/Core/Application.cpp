// ReCrafted (c) 2016-2018 Always Too Late

#include "Application.h"
#include "Scripting/Assembly.h"
#include "Scripting/Object.h"
#include "Scripting/Method.h"

SINGLETON_IMPL(Application)

void Application::OnInit()
{
    // create gamemain instance
    m_gamemain = Object::CreateInstance<Object>("ReCrafted.Game", "GameMain", Assembly::Game, false);

    // find methods
    m_init_method = m_gamemain->FindMethod("ReCrafted.Game.GameMain::Initialize");
    m_simulate_method = m_gamemain->FindMethod("ReCrafted.Game.GameMain::Simulate");
    m_update_method = m_gamemain->FindMethod("ReCrafted.Game.GameMain::Update");
    m_render_method = m_gamemain->FindMethod("ReCrafted.Game.GameMain::Render");
    m_renderui_method = m_gamemain->FindMethod("ReCrafted.Game.GameMain::RenderUI");
    m_shutdown_method = m_gamemain->FindMethod("ReCrafted.Game.GameMain::Shutdown");
}

void Application::OnDispose()
{
    m_shutdown_method->Invoke();
}

void Application::OnLoad()
{
    m_init_method->Invoke();
}

void Application::Simulate()
{
    m_simulate_method->Invoke();
}

void Application::Update()
{
    m_update_method->Invoke();
}

void Application::Render()
{
    m_render_method->Invoke();
}

void Application::RenderUI()
{
    m_renderui_method->Invoke();
}
