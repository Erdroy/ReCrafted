// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Action.h"

class MainLoop final
{
    DELETE_OPERATOR_COPY_MOVE(MainLoop)
    DELETE_CTOR_COPY(MainLoop)

private:
    Action<void> m_fixedUpdate;
    Action<void> m_update;
    Action<void> m_render;

    bool m_isRunning = false;

    bool m_isFirstFrame = true;
    double m_fixedUpdateAcc = 0.0;

    uint32_t m_targetFps = 0;

    bool m_fixedUpdateCallback = false;
    bool m_updateCallback = false;
    bool m_renderCallback = false;

public:
    MainLoop();
    MainLoop(MainLoop&& mainLoop) = default;
    ~MainLoop();

private:
    void FixedUpdate();
    void WaitForTargetFps(double& last, double current) const;

public:
    void Run();
    void Quit();

public:
     /// <summary>
     ///     Sets Update callback.
     /// </summary>
     /// <param name="callback"The Update callback delegate.></param>
    void SetUpdateCallback(const Action<void>& callback);

    /// <summary>
    ///     Sets FixedUpdate callback.
    /// </summary>
    /// <param name="callback"The FixedUpdate callback delegate.></param>
    void SetFixedUpdateCallback(const Action<void>& callback);

    /// <summary>
    ///     Sets render callback.
    /// </summary>
    /// <param name="callback"The render callback delegate.></param>
    void SetRenderCallback(const Action<void>& callback);
};
