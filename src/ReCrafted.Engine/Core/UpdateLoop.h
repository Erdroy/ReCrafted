// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef UPDATELOOP_H
#define UPDATELOOP_H

// includes
#include "Delegate.h"

/**
 * \brief UpdateLoop class. Implements generic timing logic.
 */
class UpdateLoop
{
private:
    Delegate m_simulate;
    Delegate m_update;
    Delegate m_render;

    bool m_running = true;
    double m_simulationAcc = 0.0;

    bool m_simulateCallback = false;
    bool m_updateCallback = false;
    bool m_renderCallback = false;

private:
    void simulate();
    void waitForTargetFps(double last);

public:
    /**
     * \brief Starts the loop.
     */
    void start();
    
    /**
    * \brief Stops the loop.
    */
    void stop();

public:
    /**
     * \brief Sets update callback.
     * \param callback The update callback delegate.
     */
    void setUpdateCallback(Delegate callback);

    /**
    * \brief Sets simulation callback.
    * \param callback The simulation callback delegate.
    */
    void setSimulateCallback(Delegate callback);

    /**
    * \brief Sets render callback.
    * \param callback The render callback delegate.
    */
    void setRenderCallback(Delegate callback);

public:
    PROPERTY(int, targetFps) = 60;
};

#endif // UPDATELOOP_H
