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
class Application : public EngineComponent<Application>
{
    friend class EngineMain;
    friend class Renderer;

private:
    SCRIPTING_API_IMPL()

private:
    Ptr<Object> m_gamemain = nullptr;
    Ptr<Method> m_init_method = nullptr;
    Ptr<Method> m_update_method = nullptr;
    Ptr<Method> m_simulate_method = nullptr;
    Ptr<Method> m_render_method = nullptr;
    Ptr<Method> m_renderui_method = nullptr;
    Ptr<Method> m_shutdown_method = nullptr;

public:
    virtual ~Application() = default;

private:
    void onInit() override;
    void onDispose() override;
    void onLoad() override;

private:
    void simulate();
    void update();
    void render();
    void renderUI();
};

#endif // APPLICATION_H
