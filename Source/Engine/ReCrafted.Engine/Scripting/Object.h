// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

/// <summary>
///     Object class. Base class for all mono object instances.
/// </summary>
API_CLASS(public, partial, customNamespace="ReCrafted.API")
class Object
{
    API_CLASS_BODY()

public:
    /// <summary>
    ///     Destroys given Object instance.
    /// </summary>
    API_FUNCTION()
    static void Destroy(Object* objectInstance);
};