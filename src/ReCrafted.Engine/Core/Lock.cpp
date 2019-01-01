// ReCrafted (c) 2016-2019 Always Too Late

#include "Lock.h"

#include <Windows.h>
#include <intrin.h>

// TODO: cross-platform implementation
// source: http://preshing.com/20120226/roll-your-own-lightweight-mutex/

Lock::Lock()
{
    m_counter = 0;
    m_semaphore = CreateSemaphore(nullptr, 0, 1, nullptr);
}

Lock::~Lock()
{
    CloseHandle(m_semaphore);
}

void Lock::Enter()
{
    //WaitForSingleObject(m_semaphore, INFINITE);

    // Kinda nasty way to do this, but works.
    LockNow();
    UnlockNow();
}

void Lock::LockNow()
{
    if (_InterlockedIncrement(&m_counter) > 1)
    {
        WaitForSingleObject(m_semaphore, INFINITE);
    }
}

bool Lock::TryLock()
{
    return _InterlockedCompareExchange(&m_counter, 1, 0) == 0u;
}

void Lock::UnlockNow()
{
    if (_InterlockedDecrement(&m_counter) > 0)
    {
        ReleaseSemaphore(m_semaphore, 1, nullptr);
    }
}
