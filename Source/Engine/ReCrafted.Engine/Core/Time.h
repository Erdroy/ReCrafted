// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "SubSystems/SubSystem.h"

class Time final : public SubSystem<Time>
{
    friend class MainLoop;

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
    static float CurrentTime();
    static float CurrentFixedTime();
    static float DeltaTime();
    static float FixedDeltaTime();
    static uint32_t CurrentFrame();
};
