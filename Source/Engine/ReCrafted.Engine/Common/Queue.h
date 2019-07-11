// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include <queue>

template <typename TType>
class Queue
{
private:
    std::queue<TType> m_queue;

public:
    void Enqueue(const TType& item)
    {
        m_queue.emplace(item);
    }

    TType Dequeue()
    {
        TType item = m_queue.back();
        m_queue.pop();
        return item;
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