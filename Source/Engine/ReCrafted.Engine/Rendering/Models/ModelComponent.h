// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

/// <summary>
///     The model component structure. 
///     This struct is being used to define model's rendering settings.
/// </summary>
struct ModelComponent
{
public:
    bool Free = true;
    bool Active = false;
    Mesh* Mesh = nullptr;
    Material* Material = nullptr;
    Transform* Transform = nullptr;
    BoundingSphere Bounds = {};

public:
    /// <summary>
    ///     Returns true, when this model component will be rendered.
    /// </summary>
    /// <returns></returns>
    bool IsValid() const
    {
        return Mesh && Material&& Transform;
    }
};
