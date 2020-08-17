// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include <concurrentqueue.h>

/// <summary>
///     Concurrent queue.
/// </summary>
template <typename TType>
class ConcurrentQueue
{
private:
    moodycamel::ConcurrentQueue<TType> m_queue;

public:
    void Enqueue(const TType& item)
    {
        m_queue.enqueue(item);
    }

    bool TryDequeue(TType& item)
    {
        return m_queue.try_dequeue(item);
    }

    TType Dequeue()
    {
        TType item;
        ASSERT(m_queue.try_dequeue(item));
        return item;
    }
};