// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef EVENT_H
#define EVENT_H

// includes
#include "ReCrafted.h"
#include "Action.h"
#include "Lock.h"
#include "Containers/Array.h"

template <typename ... TParams>
class Event
{
private:
    Array<Action<void, TParams...>> m_listeners = {};
    Lock m_lock = {};

public:
    void Invoke(TParams ... params)
    {
        ScopeLock(m_lock);

        // invoke all listeners
        for (var&& listener : m_listeners)
            listener.Invoke(params...);
    }

    void AddListener(Action<void, TParams...> delegate)
    {
        ScopeLock(m_lock);
        m_listeners.Add(delegate);
    }

    void RemoveListener(Action<void> delegate)
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

#endif // EVENT_H
