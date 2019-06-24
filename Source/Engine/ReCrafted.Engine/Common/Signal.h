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

private:
    void Release();
    void Reset();

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
        // Release this signal if initialized
        Release();

        m_handle = signal.m_handle;
        signal.Reset();
    }

    /// <summary>
    ///     Default destructor
    /// </summary>
    ~Signal();

public:
    /// <summary>
    ///     Emits signal. This will make all 'Wait' calls to proceed.
    /// </summary>
    void Emit() const;

    /// <summary>
    ///     Waits for signal emission.
    /// </summary>
    /// <param name="timeout">The wait timeout.</param>
    void Wait(uint32_t timeout = 0xFFFFFFFFu) const;

public:
    Signal& operator=(Signal&& signal) noexcept
    {
        // Release this signal if initialized
        Release();

        // Copy handle from other signal and reset it's values
        m_handle = signal.m_handle;
        signal.Reset();
        return *this;
    }

public:
    /// <summary>
    ///     Creates a new signal.
    /// </summary>
    static Signal Create();
};