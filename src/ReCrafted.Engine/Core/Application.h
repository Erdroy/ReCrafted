// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H

// includes
#include "ReCrafted.h"
#include "EngineComponent.h"

/**
 * \brief Application class. EngineComponent for ScriptingEngine, 
 * which makes possible to run update/simulate/render calls in Mono.
 */
class Application : public EngineComponent
{
    friend class EngineMain;

    API_DEF

private:
    static Application* m_instance;

private:
    Ptr<Object> m_gamemain = nullptr;
    Ptr<Method> m_init_method = nullptr;
    Ptr<Method> m_update_method = nullptr;
    Ptr<Method> m_simulate_method = nullptr;
    Ptr<Method> m_render_method = nullptr;
    Ptr<Method> m_renderui_method = nullptr;
    Ptr<Method> m_shutdown_method = nullptr;

public:
    Application() { m_instance = this; }

private:
    void onInit() override;
    void onShutdown() override;
    void onLoad() override;

private:
    void simulate();
    void update();
    void render();

public:
    static Application* getInstance();
};

#endif // APPLICATION_H
