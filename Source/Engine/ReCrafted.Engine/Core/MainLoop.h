// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

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
    /**
     * \brief Sets Update callback.
     * \param callback The Update callback delegate.
     */
    void SetUpdateCallback(const Action<void>& callback);

    /**
    * \brief Sets FixedUpdate callback.
    * \param callback The FixedUpdate callback delegate.
    */
    void SetFixedUpdateCallback(const Action<void>& callback);

    /**
    * \brief Sets render callback.
    * \param callback The render callback delegate.
    */
    void SetRenderCallback(const Action<void>& callback);
};
