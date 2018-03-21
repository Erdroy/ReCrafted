// ReCrafted (c) 2016-2018 Always Too Late

#include "UpdateLoop.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Time.h"

void UpdateLoop::simulate()
{
    // source: https://gafferongames.com/post/fix_your_timestep/

    const var dt = Time::fixedDeltaTime();

    m_simulationAcc += Time::deltaTime();

    while (m_simulationAcc >= dt)
    {
        m_simulate.Invoke();
        m_simulationAcc -= dt;
    }
}

void UpdateLoop::waitForTargetFps(double last)
{
    Profiler::beginProfile("WaitForTargetFps");
    cvar target = 1.0 / m_targetFps * 1000.0;
    cvar delta = Platform::getMiliseconds() - last;
    cvar sleep = target - delta;

    if (sleep > 0.0)
    {
        Platform::sleep(static_cast<uint>(sleep));
    }
    Profiler::endProfile();
}

void UpdateLoop::start()
{
    var lastTime = Platform::getMiliseconds();

    while(m_running)
    {
        cvar currentTime = Platform::getMiliseconds();

        Profiler::beginProfile("Frame");
        
        if(m_updateCallback)
        {
            // call update
            m_update.Invoke();
        }

        if(m_simulateCallback)
        {
            // run simulate
            simulate();
        }

        if(m_renderCallback)
        {
            // call render
            m_render.Invoke();
        }
        
        waitForTargetFps(lastTime);
        lastTime = currentTime;

        // end 'Frame' profile
        Profiler::endProfile();
        Profiler::endFrame();
    }
}

void UpdateLoop::stop()
{
    m_running = false;
}

void UpdateLoop::setUpdateCallback(Delegate<void> callback)
{
    m_updateCallback = true;

    // set update callback
    m_update = callback;
}

void UpdateLoop::setSimulateCallback(Delegate<void> callback)
{
    m_simulateCallback = true;

    // set simulate callback
    m_simulate = callback;
}

void UpdateLoop::setRenderCallback(Delegate<void> callback)
{
    m_renderCallback = true;

    // set render callback
    m_render = callback;
}
