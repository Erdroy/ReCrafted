// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H

// includes
#include "ReCrafted.h"
#include "EngineComponent.h"

/**
 * \brief Application class. EngineComponent for ScriptingEngine, 
 * which makes possible to run Update/Simulate/render calls in Mono.
 */
class Application : public EngineComponent<Application>
{
    friend class EngineMain;
    friend class Graphics;

private:
SCRIPTING_API_IMPL()

private:
    Object* m_gamemain = nullptr;
    RefPtr<Method> m_init_method = nullptr;
    RefPtr<Method> m_update_method = nullptr;
    RefPtr<Method> m_simulate_method = nullptr;
    RefPtr<Method> m_render_method = nullptr;
    RefPtr<Method> m_shutdown_method = nullptr;

public:
    virtual ~Application() = default;

private:
    void OnInit() override;
    void OnDispose() override;
    void OnLoad() override;
    void Update() override;

private:
    void Simulate();
    void Render();
};

#endif // APPLICATION_H
