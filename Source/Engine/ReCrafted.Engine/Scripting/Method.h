// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Mono.h"

/// <summary>
///     Method structure. Represents Mono scripting backend method.
/// </summary>
struct Method
{
private:
    MonoMethod* m_method = nullptr;

public:
    /// <summary>
    ///     Initializes Method structure instance.
    /// </summary>
    /// <param name="method">The mono method reference.</param>
    explicit Method(MonoMethod* method) : m_method(method)
    {
    }

public:
    /// <summary>
    ///     Invokes this function on given instance.
    /// </summary>
    /// <param name="instance">The instance.</param>
    /// <param name="params">The method parameters.</param>
    /// <remarks>Object instance must have the same type as this method description.</remarks>
    void Invoke(Object* instance, void** params = nullptr) const;

    /// <summary>
    ///     Invokes this function statically.
    /// </summary>
    /// <param name="params">The method parameters.</param>
    void InvokeStatic(void** params = nullptr) const;

    /// <summary>
    ///     Returns true when valid.
    /// </summary>
    bool IsValid() const;

    /// <summary>
    ///     Returns Mono method pointer.
    /// </summary>
    /// <returns>The mono's method pointer.</returns>
    MonoMethod* ToMono() const;
};