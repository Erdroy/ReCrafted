// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Mono.h"

/// <summary>
///     Class structure. Represents Mono scripting backend class.
/// </summary>
struct Class
{
private:
    MonoClass* m_class = nullptr;

public:
    /// <summary>
    ///     Initializes Class structure instance.
    /// </summary>
    /// <param name="mClass">The mono class reference.</param>
    explicit Class(MonoClass* mClass) : m_class(mClass)
    {
    }

public:
    /// <summary>
    ///     Returns base class of this case.
    /// </summary>
    /// <returns>The base class.</returns>
    Class GetBaseClass() const;

    /// <summary>
    ///     Finds method using given name.
    /// </summary>
    /// <param name="methodName">The method name.</param>
    /// <returns>The method structure.</returns>
    Method GetMethod(const char* methodName) const;

    /// <summary>
    ///     Finds field using given name.
    /// </summary>
    /// <param name="fieldName">The field name.</param>
    /// <returns>The field structure.</returns>
    Field GetField(const char* fieldName) const;

    /// <summary>
    ///     Returns true when valid.
    /// </summary>
    bool IsValid() const;

    /// <summary>
    ///     Returns name of this class.
    /// </summary>
    char* Name() const;

    /// <summary>
    ///     Returns Mono class pointer.
    /// </summary>
    /// <returns>The mono's class pointer.</returns>
    MonoClass* ToMono() const;
};