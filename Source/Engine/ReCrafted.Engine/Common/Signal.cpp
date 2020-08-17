// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Signal.h"

// TODO: cross-platform implementation

#if defined(_WIN32)

#include <Windows.h>

void Signal::Release()
{
    if(m_handles)
    {
        for (auto i = 0; i < m_numHandles; i++)
        {
            if (!m_handles[i]) continue;

            CloseHandle(m_handles[i]);
            m_handles[i] = nullptr;
        }

        delete[] m_handles;
    }
    else if(m_handle)
    {
        CloseHandle(m_handle);
    }

    m_handle = nullptr;
    m_handles = nullptr;
}

Signal::Signal(const uint16_t numHandles)
{
    ASSERT(numHandles > 0);

    m_numHandles = numHandles;
    m_handles = new HANDLE[numHandles];

    for (auto i = 0; i < m_numHandles; i++)
        m_handles[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

Signal::~Signal()
{
    Release();
}

void Signal::Reset() const
{
    if (m_handles)
    {
        for (auto i = 0; i < m_numHandles; i++)
            ResetEvent(m_handles[i]);
    }
    else if (m_handle)
    {
        ResetEvent(m_handle);
    }
}

void Signal::Emit(const uint16_t handle) const
{
    if(m_handles)
    {
        ASSERT(handle < m_numHandles);
        ASSERT(m_handles[handle]);
        SetEvent(m_handles[handle]);
    }
    else
    {
        ASSERT(m_handle);
        SetEvent(m_handle);
    }
}

void Signal::Wait(const uint32_t timeout) const
{
    ASSERT(m_handle);
    WaitForSingleObject(m_handle, static_cast<DWORD>(timeout));
}

void Signal::WaitMultiple(const uint32_t timeout) const
{
    ASSERT(m_handles);
    ASSERT(m_numHandles > 0);
    WaitForMultipleObjects(m_numHandles, m_handles, TRUE, static_cast<DWORD>(timeout));
}

Signal Signal::Create()
{
    Signal signal;
    signal.m_handle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    return signal;
}

#endif
