// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Action.h"
#include "Lock.h"
#include "List.h"

template <typename ... TParams>
class Event
{
private:
    List<Action<void, TParams...>> m_listeners = {};
    Lock m_lock = {};

public:
    void Invoke(TParams ... params)
    {
        ScopeLock(m_lock);

        // invoke all listeners
        for (auto&& listener : m_listeners)
            listener.Invoke(params...);
    }

    void AddListener(const Action<void, TParams...>& delegate)
    {
        ScopeLock(m_lock);
        m_listeners.Add(delegate);
    }

    void RemoveListener(const Action<void>& delegate)
    {
        ScopeLock(m_lock);
        m_listeners.Remove(delegate);
    }

    void RemoveListeners()
    {
        ScopeLock(m_lock);
        m_listeners.clear();
    }
};
