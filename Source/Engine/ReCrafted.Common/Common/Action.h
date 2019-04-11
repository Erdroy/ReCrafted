// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#ifndef ACTION_H
#define ACTION_H

#include <ReCrafted.h>

template<typename TReturn, typename ... TArgs>
struct Action
{
private:
    typedef void* InstancePtr;
    typedef TReturn(*FunctionSignature)(TArgs...);
    typedef TReturn(*FunctionSignatureProxy)(InstancePtr, TArgs...);

private:
    template <class TBase, TReturn(TBase::*Function)(TArgs...)>
    static TReturn FunctionProxy(InstancePtr instance, TArgs... args)
    {
        return (static_cast<TBase*>(instance)->*Function)(args...);
    }

private:
    FunctionSignature m_free_function = nullptr;
    FunctionSignatureProxy m_proxy_function = nullptr;
    InstancePtr m_proxy_instance = nullptr;

    std::function<TReturn(TArgs...)> m_stdfunc = nullptr;

private:
    template<typename T, typename... U>
    size_t getFunctionAddress(std::function<T(U...)> f) {
        typedef T(fnType)(U...);
        fnType ** fnPointer = f.template target<fnType*>();
        return (size_t)*fnPointer;
    }

public:
    Action();

    Action(const Action<TReturn, TArgs...>& action)
    {
        m_free_function = action.m_free_function;
        m_proxy_instance = action.m_proxy_instance;
        m_proxy_function = action.m_proxy_function;
        m_stdfunc = action.m_stdfunc;
    }

    Action(FunctionSignature function)
    {
        m_free_function = function;
    }

    Action(std::function<TReturn(TArgs...)> stdFunc)
    {
        m_stdfunc = stdFunc;
    }

    ~Action();

public:
    template<typename TBase, TReturn(TBase::*Function)(TArgs...)>
    void Set(TBase* instance)
    {
        ASSERT(m_proxy_instance == nullptr);
        ASSERT(m_proxy_function == nullptr);
        ASSERT(instance != nullptr);

        m_proxy_instance = static_cast<InstancePtr>(instance);
        m_proxy_function = &FunctionProxy<TBase, Function>;
    }

    TReturn Invoke(TArgs... args)
    {
        if (m_proxy_function)
        {
            DEBUG_ASSERT(m_proxy_instance);
            return m_proxy_function(m_proxy_instance, args...);
        }

        if(m_stdfunc)
        {
            return m_stdfunc(args...);
        }

        DEBUG_ASSERT(m_free_function);
        return m_free_function(args...);
    }

public:
    TReturn operator()(TArgs... args)
    {
        if (m_proxy_function)
        {
            DEBUG_ASSERT(m_proxy_instance);
            return m_proxy_function(m_proxy_instance, args...);
        }

        if (m_stdfunc)
        {
            return m_stdfunc(args...);
        }

        DEBUG_ASSERT(m_free_function);
        return m_free_function(args...);
    }

    bool operator ==(const Action<TReturn, TArgs...>& other)
    {
        return m_free_function == other.m_free_function
            && getFunctionAddress(m_stdfunc) == getFunctionAddress(other.m_stdfunc)
            && m_proxy_instance == other.m_proxy_instance
            && m_proxy_function == other.m_proxy_function;
    }

    Action<TReturn, TArgs...>& operator=(const Action<TReturn, TArgs...>& other)
    {
        if (&other == this)
            return *this;
        
        m_free_function = other.m_free_function;
        m_proxy_instance = other.m_proxy_instance;
        m_proxy_function = other.m_proxy_function;
        m_stdfunc = other.m_stdfunc;
        return *this;
    }

public:
    bool IsValid()
    {
        return m_free_function || (m_proxy_function && m_proxy_instance);
    }

public:
    template<typename TBase, TReturn(TBase::*Function)(TArgs...)>
    static Action<TReturn, TArgs...> New(TBase* instance)
    {
        Action<TReturn, TArgs...> action = Action<TReturn, TArgs...>();
        action.Set<TBase, Function>(instance);
        return action;
    }

    template<typename TMakeReturn, typename ... TMakeArgs>
    static Action<TMakeReturn, TMakeArgs...> New(TMakeReturn(*function)(TMakeArgs...))
    {
        return Action<TMakeReturn, TMakeArgs...>(function);
    }
};

template <typename TReturn, typename ... TArgs>
Action<TReturn, TArgs...>::Action() = default;

template <typename TReturn, typename ... TArgs>
Action<TReturn, TArgs...>::~Action() = default;
#endif // ACTION_H
