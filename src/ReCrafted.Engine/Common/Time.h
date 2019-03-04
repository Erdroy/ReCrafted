// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef TIME_H
#define TIME_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Common/ReCraftedAPI.h"

/**
 * \brief Time class. Implements Time components and API.
 */
class Time : public EngineComponent<Time>
{
    friend class EngineMain;

private:
SCRIPTING_API_IMPL()


private:
    double m_deltaTime = 0.0;
    double m_fixedDeltaTime = 1.0 / 60.0;

    float m_time = 0.0;
    float m_fixedTime = 0.0;
    int m_frames = 0;

    double m_lastUpdateTime = 0.0;

private:
    void OnInit() override
    {
    }

    void OnDispose() override
    {
        m_deltaTime = 0.0f;
    }

    void OnFrame();

public:
    /**
	 * \brief The current frame delta time.
	 * Use for 'Update' calls.
	 * \return The frame delta time.
	 */
    static double DeltaTime();

    /**
     * \brief The fixed simulation delta time. Use for 'Simulate' calls.
     * \return The fixed delta time value.
     */
    static double FixedDeltaTime();

    /**
    * \brief The fixed simulation delta time. Use for 'Simulate' calls.
    * \return The fixed delta time value.
    */
    static void SetFixedDeltaTime(double fixedDeltaTime);

    /**
    * \brief The current game time.
    * \return The time.
    */
    static float CurrentTime();

    /**
	 * \brief The current game simulation time.
	 * \return The time.
	 */
    static float CurrentFixedTime();

    /**
	 * \brief The current game frame count.
	 * \return The frame count.
	 */
    static int Frames();
};

#endif // TIME_H
