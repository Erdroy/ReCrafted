// ReCrafted (c) 2016-2018 Always Too Late

#include "UpdateLoop.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Time.h"

void UpdateLoop::Simulate()
{
    // source: https://gafferongames.com/post/fix_your_timestep/

    const var dt = Time::FixedDeltaTime();

    m_simulationAcc += Time::DeltaTime();

    while (m_simulationAcc >= dt)
    {
        m_simulate.Invoke();
        m_simulationAcc -= dt;
    }
}

void UpdateLoop::WaitForTargetFps(double last)
{
    Profiler::BeginProfile("WaitForTargetFps");
    cvar target = (1.0 / m_TargetFps) * 1000.0;
    cvar delta = Platform::GetMiliseconds() - last;
    cvar sleep = target - delta;

    if (sleep > 0.0)
    {
        Platform::Sleep(static_cast<uint>(sleep));
    }
    Profiler::EndProfile();
}

void UpdateLoop::Start()
{
    var lastTime = Platform::GetMiliseconds();

    while (m_running)
    {
        cvar currentTime = Platform::GetMiliseconds();

        Profiler::BeginProfile("Frame Total");
        Profiler::BeginProfile("Frame");

        if (m_updateCallback)
        {
            // call Update
            m_update.Invoke();
        }

        if (m_simulateCallback)
        {
            // run Simulate
            Simulate();
        }

        if (m_renderCallback)
        {
            // call render
            m_render.Invoke();
        }

        Profiler::EndProfile();

        WaitForTargetFps(lastTime);
        lastTime = currentTime;

        // end 'Frame' profile
        Profiler::EndProfile();
        Profiler::GetInstance()->EndFrame();
    }
}

void UpdateLoop::Stop()
{
    m_running = false;
}

void UpdateLoop::SetUpdateCallback(Action<void> callback)
{
    m_updateCallback = true;

    // set Update callback
    m_update = callback;
}

void UpdateLoop::SetSimulateCallback(Action<void> callback)
{
    m_simulateCallback = true;

    // set Simulate callback
    m_simulate = callback;
}

void UpdateLoop::SetRenderCallback(Action<void> callback)
{
    m_renderCallback = true;

    // set render callback
    m_render = callback;
}
