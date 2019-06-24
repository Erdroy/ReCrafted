// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Signal.h"

// TODO: cross-platform implementation

#if defined(_WIN32)

#include <Windows.h>

void Signal::Release()
{
    if (m_handle != nullptr)
    {
        CloseHandle(m_handle);
        Reset();
    }
}

void Signal::Reset()
{
    m_handle = nullptr;
}

Signal::Signal()
{
    m_handle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

Signal::~Signal()
{
    Release();
}

void Signal::Emit() const
{
    ASSERT(m_handle);
    SetEvent(m_handle);
}

void Signal::Wait(const uint32_t timeout) const
{
    ASSERT(m_handle);
    WaitForSingleObject(m_handle, static_cast<DWORD>(timeout));
}

#endif
