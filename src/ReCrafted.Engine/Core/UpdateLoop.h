// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef UPDATELOOP_H
#define UPDATELOOP_H

// includes
#include "ReCrafted.h"
#include "Action.h"

/**
 * \brief UpdateLoop class. Implements generic timing logic.
 */
class UpdateLoop
{
private:
    Action<void> m_simulate;
    Action<void> m_update;
    Action<void> m_render;

    bool m_running = true;
    bool m_isFirstFrame = true;
    double m_simulationAcc = 0.0;

    bool m_simulateCallback = false;
    bool m_updateCallback = false;
    bool m_renderCallback = false;

private:
    void Simulate();
    void WaitForTargetFps(double& last, double current);

public:
    /**
     * \brief Starts the loop.
     */
    void Start();

    /**
    * \brief Stops the loop.
    */
    void Stop();

public:
    /**
     * \brief Sets Update callback.
     * \param callback The Update callback delegate.
     */
    void SetUpdateCallback(Action<void> callback);

    /**
    * \brief Sets simulation callback.
    * \param callback The simulation callback delegate.
    */
    void SetSimulateCallback(Action<void> callback);

    /**
    * \brief Sets render callback.
    * \param callback The render callback delegate.
    */
    void SetRenderCallback(Action<void> callback);

public:
PROPERTY(int, TargetFps) = 60;
};

#endif // UPDATELOOP_H
