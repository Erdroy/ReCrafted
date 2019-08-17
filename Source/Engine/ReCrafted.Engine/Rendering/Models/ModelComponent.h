// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Action.h"
#include "Common/List.h"

/// <summary>
///     The model component structure. 
///     This struct is being used to define model's rendering settings.
/// </summary>
struct ModelComponent
{
public:
    bool Free = true;
    bool Active = false;
    bool Procedural = false;
    Material* Material = nullptr;
    Transform* Transform = nullptr;
    BoundingSphere Bounds = {};
    List<Mesh*> Meshes = {};

public:
    /// <summary>
    ///     OnBeginRender callback called when this model is being rendered. Only called when this is procedural model component,
    ///     created by ModelRenderingSystem::AcquireModelComponent(true).
    ///     Can be used to change the Material, Transform etc.
    /// </summary>
    Action<void, int> OnBeginRender;

public:
    /// <summary>
    ///     Returns true, when this model component will be rendered.
    /// </summary>
    /// <returns></returns>
    bool IsValid() const
    {
        return Meshes.Count() > 0 && Material /*&& Transform*/;
    }
};
