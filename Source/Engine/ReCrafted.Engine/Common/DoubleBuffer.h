// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "Common/Lock.h"

/// <summary>
///     Thread-safe double buffer container. Allows to safely swap items.
/// </summary>
template<typename TItem>
class DoubleBuffer
{
private:
    TItem m_first;
    TItem m_second;

    Lock m_lock;

public:
    DoubleBuffer(TItem first, TItem second) : m_first(first), m_second(second) {}

public:
    /// <summary>
    ///     Swaps items and returns the first available one.
    /// </summary>
    /// <returns>The item.</returns>
    TItem Swap()
    {
        ScopeLock(m_lock);
        std::swap(m_first, m_second);
        return m_first;
    }

    /// <summary>
    ///     Returns the peak item.
    /// </summary>
    /// <returns>The item.</returns>
    TItem Peak()
    {
        ScopeLock(m_lock);
        return m_first;
    }

    /// <summary>
    ///     Returns the back item.
    /// </summary>
    /// <returns>The item.</returns>
    TItem Back()
    {
        ScopeLock(m_lock);
        return m_second;
    }

    /// <summary>
    ///     Sets the peak item.
    /// </summary>
    /// <param name="item">The new peak item.</param>
    void SetPeak(TItem item)
    {
        ScopeLock(m_lock);
        m_first = item;
    }

    /// <summary>
    ///     Sets the back item.
    /// </summary>
    /// <param name="item">The new back item.</param>
    void SetBack(TItem item)
    {
        ScopeLock(m_lock);
        m_second = item;
    }
};