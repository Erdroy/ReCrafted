// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Mono.h"

class Assembly;

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
    ///     Returns name of this class.
    /// </summary>
    char* Name() const;

    /// <summary>
    ///     Returns Mono class pointer.
    /// </summary>
    /// <returns>The mono's class pointer.</returns>
    MonoClass* ToMono() const;
};