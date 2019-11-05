// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

/// <summary>
///     Signal class. Provides signal emission and wait for emission functionality.
///     Allows easy thread synchronization and more.
/// </summary>
struct Signal
{
    DELETE_CTOR_COPY(Signal)
    DELETE_OPERATOR_COPY(Signal)

    HANDLE m_handle = nullptr;

    uint16_t m_numHandles = 0;
    HANDLE* m_handles = nullptr;

private:
    void Release();

public:
    /// <summary>
    ///     Default constructor
    /// </summary>
    Signal() = default;

    /// <summary>
    ///     Move constructor
    /// </summary>
    Signal(Signal&& signal) noexcept
    {
        m_handle = nullptr;
        m_handles = nullptr;

        m_handle = signal.m_handle;
        m_handles = signal.m_handles;
        m_numHandles = signal.m_numHandles;

        signal.m_handle = nullptr;
        signal.m_handles = nullptr;
    }

    /// <summary>
    ///     Signal constructor for multiple handles.
    /// </summary>
    explicit Signal(uint16_t numHandles);

    /// <summary>
    ///     Default destructor
    /// </summary>
    ~Signal();

public:
    /// <summary>
    ///     Resets signal. Sets all handles to be un-set, so Wait* will wait for new emission batch.
    /// </summary>
    void Reset() const;

    /// <summary>
    ///     Emits signal. This will make all 'Wait' calls to proceed.
    /// </summary>
    void Emit(uint16_t handle = 0u) const;

    /// <summary>
    ///     Waits for signal emission.
    /// </summary>
    /// <param name="timeout">The wait timeout.</param>
    void Wait(uint32_t timeout = 0xFFFFFFFFu) const;

    /// <summary>
    ///     Waits for signal emission by all handles.
    /// </summary>
    /// <param name="timeout">The wait timeout.</param>
    void WaitMultiple(uint32_t timeout = 0xFFFFFFFFu) const;

public:
    Signal& operator=(Signal&& signal) noexcept
    {
        m_handle = nullptr;
        m_handles = nullptr;

        // Copy handle from other signal and reset it's values
        m_handle = signal.m_handle;
        m_handles = signal.m_handles;
        m_numHandles = signal.m_numHandles;
        signal.m_handle = nullptr;
        signal.m_handles = nullptr;
        return *this;
    }

public:
    /// <summary>
    ///     Creates a new signal.
    /// </summary>
    static Signal Create();
};