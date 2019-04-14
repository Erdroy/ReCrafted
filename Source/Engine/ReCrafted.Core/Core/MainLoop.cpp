// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "MainLoop.h"
#include "Common/Platform/Platform.h"
#include "Core/Time.h"

MainLoop::MainLoop()
{
}

MainLoop::~MainLoop()
{
}

void MainLoop::FixedUpdate()
{
    // source: https://gafferongames.com/post/fix_your_timestep/

    const auto dt = Time::FixedDeltaTime();
    auto forcePushSimulation = false;

    m_fixedUpdateAcc += Time::DeltaTime();

    if (m_isFirstFrame)
    {
        m_fixedUpdateAcc = 0.0;
        m_isFirstFrame = false;
        forcePushSimulation = true;
    }

    while (m_fixedUpdateAcc >= dt || forcePushSimulation)
    {
        m_fixedUpdate.Invoke();
        m_fixedUpdateAcc -= dt;

        forcePushSimulation = false;
    }
}

void MainLoop::WaitForTargetFps(double& last, const double current) const
{
    const auto target = (1.0 / m_targetFps) * 1000.0;
    const auto delta = Platform::GetMilliseconds() - last;
    const auto sleep = target - delta;

    if (sleep > 0.0)
        Platform::Sleep(static_cast<uint>(sleep));

    last = current;
}

void MainLoop::Run()
{
    m_isRunning = true;

    auto lastFrameStart = Platform::GetMilliseconds();

    while(m_isRunning)
    {
        const auto currentFrameStart = Platform::GetMilliseconds();
        Time::GetInstance()->OnFrame();

        if (m_updateCallback)
            m_update.Invoke();

        if (m_fixedUpdateCallback)
            FixedUpdate();

        if (m_renderCallback)
            m_render.Invoke();

        // Wait for target fps
        if (m_targetFps != 0)
            WaitForTargetFps(lastFrameStart, currentFrameStart);
    }
}

void MainLoop::Quit()
{
    // Quit the main loop next frame
    m_isRunning = false;
}

void MainLoop::SetUpdateCallback(const Action<void>& callback)
{
    m_updateCallback = true;

    // set Update callback
    m_update = callback;
}

void MainLoop::SetFixedUpdateCallback(const Action<void>& callback)
{
    m_fixedUpdateCallback = true;

    // set FixedUpdate callback
    m_fixedUpdate = callback;
}

void MainLoop::SetRenderCallback(const Action<void>& callback)
{
    m_renderCallback = true;

    // set render callback
    m_render = callback;
}