// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef EVENT_H
#define EVENT_H

// includes
#include "ReCrafted.h"
#include "Action.h"
#include "Lock.h"
#include "Containers/Array.h"

template <typename T>
class Event
{
private:
    Array<Action<T>> m_listeners = {};
    Lock m_lock = {};

public:
    void Invoke()
    {
        ScopeLock(m_lock);

        // invoke all listeners
        for (var&& listener : m_listeners)
            listener.Invoke();
    }

    void Invoke(T* param)
    {
        ScopeLock(m_lock);

        // invoke all listeners
        for (var&& listener : m_listeners)
            listener.Invoke(param);
    }

    void AddListener(Action<T> delegate)
    {
        ScopeLock(m_lock);
        m_listeners.Add(delegate);
    }

    void RemoveListener(Action<T> delegate)
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
