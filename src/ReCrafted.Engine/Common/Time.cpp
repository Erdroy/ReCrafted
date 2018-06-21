// ReCrafted (c) 2016-2018 Always Too Late

#include "Time.h"

SINGLETON_IMPL(Time)

void Time::OnFrame()
{
    cvar currentTime = Platform::GetMiliseconds();
    m_deltaTime = (currentTime - m_lastUpdateTime) / 1000.0;
    m_time = float(currentTime / 1000.0);
    m_lastUpdateTime = currentTime;
}
