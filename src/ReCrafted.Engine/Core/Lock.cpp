// ReCrafted © 2016-2017 Always Too Late

#include "Lock.h"

#include <windows.h>
#include <intrin.h>

// TODO: cross-platform implementation
// source: http://preshing.com/20120226/roll-your-own-lightweight-mutex/

Lock::Lock()
{
	m_counter = 0;
	m_semaphore = CreateSemaphore(nullptr, 0, 1, nullptr);
}

void Lock::lock()
{
	if (_InterlockedIncrement(&m_counter) > 1)
	{
		WaitForSingleObject(m_semaphore, INFINITE);
	}
}

bool Lock::tryLock()
{
	return _InterlockedCompareExchange(&m_counter, 1, 0) == 0u;
}

void Lock::unlock()
{
	if (_InterlockedDecrement(&m_counter) > 0)
	{
		ReleaseSemaphore(m_semaphore, 1, nullptr);
	}
}

void Lock::dispose()
{
	CloseHandle(m_semaphore);
}
