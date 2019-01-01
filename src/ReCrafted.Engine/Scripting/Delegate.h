// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef DELEGATE_H
#define DELEGATE_H

#include "ReCrafted.h"
#include "Scripting/ScriptingAPI.h"

struct Delegate
{
private:
    MonoObject* m_delegate = nullptr;

public:
    void* Invoke(void** params = nullptr) const
    {
        MonoObject* exception = nullptr;
        cvar returnObject = mono_runtime_delegate_invoke(m_delegate, params, &exception);

        // TODO: Raise exception

        return mono_object_unbox(returnObject);
    }

    template<typename TReturn>
    TReturn Invoke() const
    {
        // Unbox returning value
        return *reinterpret_cast<TReturn*>(Invoke());
    }

    template<typename TReturn, typename T1>
    TReturn Invoke(T1 t1) const
    {
        var data = { &t1 };

        // Unbox returning value
        return *reinterpret_cast<TReturn*>(Invoke(reinterpret_cast<void**>(&data)));
    }

    template<typename TReturn, typename T1, typename T2>
    TReturn Invoke(T1 t1, T2 t2) const
    {
        struct
        {
            T1* a;
            T2* b;
        } data;

        data.a = &t1;
        data.b = &t2;

        // Unbox returning value
        return *static_cast<TReturn*>(Invoke(reinterpret_cast<void**>(&data)));
    }

    template<typename TReturn, typename T1, typename T2, typename T3>
    TReturn Invoke(T1 t1, T2 t2, T3 t3) const
    {
        struct
        {
            T1* a;
            T2* b;
            T3* c;
        } data;

        data.a = &t1;
        data.b = &t2;
        data.c = &t3;

        // Unbox returning value
        return *static_cast<TReturn*>(Invoke(reinterpret_cast<void**>(&data)));
    }

public:
    /**
     * \brief Creates new delegate from specified mono object (delegate)/
     * \param object The mono object (delegate).
     * \return The created Delegate handle.
     */
    static Delegate FromMonoObject(MonoObject* object)
    {
        Delegate delegate;
        delegate.m_delegate = object;
        return delegate;
    }
};

#endif // DELEGATE_H
