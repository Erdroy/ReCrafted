// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SINGLETON_H
#define SINGLETON_H

// includes
#include "IDisposable.h"

/**
 * \brief Singleton class.
 * \tparam T The singleton's handled object type
 * 
 * \note PLEASE remember to add SINGLETON_IMPL(NAME_OF_CLASS) to the source file!
 * To release a Singleton, please call 'Dispose' function.
 */
template <class T>
class Singleton : IDisposable
{
protected:
    static T* m_instance;

protected:
    Singleton()
    {
    }

    ~Singleton()
    {
    }

private:
    virtual void OnDispose() = 0;

public:
    void Dispose() override
    {
        if (!m_instance)
            return;

        OnDispose();
        delete m_instance;
    }

public:
    static T* GetInstance()
    {
        if (m_instance)
            return m_instance;

        m_instance = new T;
        return m_instance;
    }
};

#define SINGLETON_IMPL(x) x* Singleton<##x##>::m_instance;

#endif // SINGLETON_H
