// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef DELEGATE_H
#define DELEGATE_H

// includes
#include "ReCrafted.h"

struct IDelegateHandler {};

/**
 * \brief Delegate class.
 */
template<typename T>
class Delegate
{
public:
    typedef void(IDelegateHandler::*delegate_t)(T* param);

private:
	IDelegateHandler* m_instance = nullptr;
    delegate_t m_delegate = nullptr;

public:
	Delegate() = default;
    Delegate(IDelegateHandler* instance, delegate_t delegate) : m_instance(instance), m_delegate(delegate) { }

public:
    FORCEINLINE void Invoke() const
    {
        if (this->m_instance == nullptr)
            return;

        (this->m_instance->*this->m_delegate)(nullptr);
    }

    FORCEINLINE void Invoke(T* param) const
    {
        if (this->m_instance == nullptr)
            return;

        (this->m_instance->*this->m_delegate)(param);
    }

public:
    bool isValid() const
    {
        return m_instance && m_delegate;
    }

public:
    bool operator == (const Delegate<T>& second)
    {
        return m_delegate == second.m_delegate && m_instance == second.m_instance;
    }
};

#define MakeDelegate(func) Delegate<void>((IDelegateHandler*)this, (Delegate<void>::delegate_t)&func)

#endif // DELEGATE_H
