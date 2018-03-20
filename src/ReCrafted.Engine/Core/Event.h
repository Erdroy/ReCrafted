// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef EVENT_H
#define EVENT_H

// includes
#include "ReCrafted.h"
#include "Delegate.h"
#include "Lock.h"
#include "Containers/Array.h"

template<typename T>
class Event
{
private:
    Array<Delegate<T>> m_listeners = {};
    Lock m_lock = {};

public:
    void invoke()
    {
        ScopeLock(m_lock);

        // invoke all listeners
        for (var && listener : m_listeners)
            listener.Invoke();
    }

    void invoke(T* param)
    {
        ScopeLock(m_lock);

        // invoke all listeners
        for (var && listener : m_listeners)
            listener.Invoke(param);
    }

    void addListener(Delegate<T> delegate)
    {
        ScopeLock(m_lock);
        m_listeners.add(delegate);
    }

    void removeListener(Delegate<T> delegate)
    {
        ScopeLock(m_lock);
        m_listeners.remove(delegate);
    }

    void removeListeners()
    {
        ScopeLock(m_lock);
        m_listeners.clear();
    }
};

#endif // EVENT_H
