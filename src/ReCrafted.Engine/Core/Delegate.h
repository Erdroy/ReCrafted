// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef DELEGATE_H
#define DELEGATE_H

// includes
#include "ReCrafted.h"

struct IDelegateHandler
{
};

/**
 * \brief Delegate class.
 */
template <typename Type>
class Delegate
{
public:
    typedef void (IDelegateHandler::*delegate_t)(Type);

private:
    IDelegateHandler* m_instance = nullptr;
    delegate_t m_delegate = nullptr;

public:
    Delegate() = default;

    Delegate(void* instance, delegate_t delegate) : m_delegate(delegate)
    {
        m_instance = (IDelegateHandler*)instance;
    }

public:
    FORCEINLINE void Invoke() const
    {
        if (m_instance == nullptr)
            return;

        (m_instance->*m_delegate)();
    }

    FORCEINLINE void Invoke(Type* param) const
    {
        if (m_instance == nullptr)
            return;

        (m_instance->*m_delegate)(param);
    }

public:
    bool isValid() const
    {
        return m_instance && m_delegate;
    }

public:
    bool operator ==(const Delegate<Type>& second)
    {
        return m_delegate == second.m_delegate && m_instance == second.m_instance;
    }
};

#define MakeDelegate(func) Delegate<void>(this, (Delegate<void>::delegate_t)&func)

#endif // DELEGATE_H
