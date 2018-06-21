// ReCrafted (c) 2016-2018 Always Too Late

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
    FORCEINLINE static double DeltaTime()
    {
        return m_instance->m_deltaTime;
    }

    /**
     * \brief The fixed simulation delta time. Use for 'Simulate' calls.
     * \return The fixed delta time value.
     */
    FORCEINLINE static double FixedDeltaTime()
    {
        return m_instance->m_fixedDeltaTime;
    }

    /**
    * \brief The fixed simulation delta time. Use for 'Simulate' calls.
    * \return The fixed delta time value.
    */
    FORCEINLINE static void SetFixedDeltaTime(double fixedDeltaTime)
    {
        m_instance->m_fixedDeltaTime = fixedDeltaTime;
    }

    /**
    * \brief The current game time.
    * \return The time.
    */
    FORCEINLINE static float CurrentTime()
    {
        return m_instance->m_time;
    }

    /**
	 * \brief The current game simulation time.
	 * \return The time.
	 */
    FORCEINLINE static float CurrentFixedTime()
    {
        return m_instance->m_fixedTime;
    }

    /**
	 * \brief The current game frame count.
	 * \return The frame count.
	 */
    FORCEINLINE static int Frames()
    {
        return m_instance->m_frames;
    }
};

#endif // TIME_H
