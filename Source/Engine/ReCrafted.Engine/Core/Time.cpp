// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Time.h"
#include "Common/Platform/Platform.h"

void Time::Initialize()
{
}

void Time::Shutdown()
{
}

void Time::OnFrame()
{
    const auto currentTime = Platform::GetMilliseconds();
    m_deltaTime = float((currentTime - m_lastUpdateTime) / 1000.0);
    m_time = float(currentTime / 1000.0);
    m_lastUpdateTime = currentTime;

    m_frames++;
}

float Time::CurrentTime()
{
    MAIN_THREAD_ONLY();
    return GetInstance()->m_time;
}

float Time::CurrentFixedTime()
{
    MAIN_THREAD_ONLY();
    return GetInstance()->m_fixedTime;
}

float Time::DeltaTime()
{
    MAIN_THREAD_ONLY();
    return GetInstance()->m_deltaTime;
}

float Time::FixedDeltaTime()
{
    MAIN_THREAD_ONLY();
    return GetInstance()->m_fixedDeltaTime;
}

uint32_t Time::CurrentFrame()
{
    MAIN_THREAD_ONLY();
    return GetInstance()->m_frames;
}
