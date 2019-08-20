// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include <queue>

/// <summary>
///     LIFO queue.
/// </summary>
template <typename TType>
class Queue
{
private:
    std::deque<TType> m_queue;

public:
    void Enqueue(const TType& item)
    {
        m_queue.emplace_back(item);
    }

    bool TryDequeue(TType& item)
    {
        if (m_queue.empty())
            return false;

        item = m_queue.back();
        m_queue.pop_back();
        return true;
    }

    bool Empty()
    {
        return m_queue.empty();
    }

    size_t Size()
    {
        return m_queue.size();
    }
};