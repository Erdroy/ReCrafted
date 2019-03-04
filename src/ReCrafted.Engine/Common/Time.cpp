// ReCrafted (c) 2016-2019 Always Too Late

#include "Time.h"
#include "Scripting/ScriptingAPI.h"

SINGLETON_IMPL(Time)

void Time::OnFrame()
{
    cvar currentTime = Platform::GetMiliseconds();
    m_deltaTime = (currentTime - m_lastUpdateTime) / 1000.0;
    m_time = float(currentTime / 1000.0);
    m_lastUpdateTime = currentTime;
}

double Time::DeltaTime()
{
    MAIN_THREAD_ONLY();
    return m_instance->m_deltaTime;
}

double Time::FixedDeltaTime()
{
    MAIN_THREAD_ONLY();
    return m_instance->m_fixedDeltaTime;
}

void Time::SetFixedDeltaTime(const double fixedDeltaTime)
{
    MAIN_THREAD_ONLY();
    m_instance->m_fixedDeltaTime = fixedDeltaTime;
}

float Time::CurrentTime()
{
    MAIN_THREAD_ONLY();
    return m_instance->m_time;
}

float Time::CurrentFixedTime()
{
    MAIN_THREAD_ONLY();
    return m_instance->m_fixedTime;
}

int Time::Frames()
{
    MAIN_THREAD_ONLY();
    return m_instance->m_frames;
}
