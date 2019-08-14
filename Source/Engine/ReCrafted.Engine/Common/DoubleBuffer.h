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
    ///     Returns the first available item.
    /// </summary>
    /// <returns>The item.</returns>
    TItem Peak()
    {
        ScopeLock(m_lock);
        return m_first;
    }
};