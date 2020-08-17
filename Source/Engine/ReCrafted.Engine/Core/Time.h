// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "SubSystems/SubSystem.h"

/// <summary>
///     The time class. Provides engine's timing services.
/// </summary>
API_CLASS(public, static, partial, noinherit)
class Time final : public SubSystem<Time>
{
    API_CLASS_BODY()
    friend class MainLoop;

public:
    const static int MaxDeltaMillis = 100;

private:
    float m_deltaTime = 0.0f;
    float m_fixedDeltaTime = 1.0f / 60.0f;

    float m_time = 0.0;
    float m_fixedTime = 0.0;
    uint32_t m_frames = 0;

    double m_lastUpdateTime = 0.0;

protected:
    void Initialize() override;
    void Shutdown() override;

private:
    void OnFrame();

public:
    /// <summary>
    ///     Returns current frame engine's time.
    /// </summary>
    API_FUNCTION(extern);
    static float CurrentTime();

    /// <summary>
    ///     Returns current frame fixed engine's time.
    /// </summary>
    API_FUNCTION(extern);
    static float CurrentFixedTime();

    /// <summary>
    ///     Returns the time between last frame and current one.
    /// </summary>
    API_FUNCTION(extern);
    static float DeltaTime();

    /// <summary>
    ///     Returns the fixed time. This time is being used as time step
    ///     for the FixedUpdate function calls.
    /// </summary>
    API_FUNCTION(extern);
    static float FixedDeltaTime();

    /// <summary>
    ///     Returns the frame alpha value.
    /// </summary>
    API_FUNCTION(extern);
    static float FrameAlpha();

    /// <summary>
    ///     Returns the precise real time since startup in seconds.
    /// </summary>
    API_FUNCTION(extern);
    static double Realtime();

    /// <summary>
    ///     The current frame number.
    /// </summary>
    API_FUNCTION(extern);
    static uint32_t CurrentFrame();
};
