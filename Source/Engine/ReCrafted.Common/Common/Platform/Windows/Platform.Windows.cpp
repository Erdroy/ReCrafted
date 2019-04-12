// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#ifdef _WIN32

#include "Common/Platform/Platform.h"

#include <Windows.h>

std::thread::id g_mainThread;

static LARGE_INTEGER m_frequency;
static double m_start;
static double m_freqCoefficient;

void Platform::Initialize()
{
    // set current thread as main
    g_mainThread = std::this_thread::get_id();

    // initialize timer
    QueryPerformanceFrequency(&m_frequency);
    m_freqCoefficient = double(m_frequency.QuadPart) / 1000.0;

    m_start = GetMilliseconds();
}

void Platform::Shutdown()
{
}

double Platform::GetMilliseconds()
{
    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);
    return double(current.QuadPart) / m_freqCoefficient - m_start;
}

void Platform::Sleep(const uint32_t milliseconds)
{
    ::Sleep(milliseconds);
}

std::thread::id Platform::GetMainThreadId()
{
    return g_mainThread;
}

void Platform::ReportAssert(String expression, String fileName, unsigned line, String message)
{

}

#endif
