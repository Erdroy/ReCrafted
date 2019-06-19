// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Rendering/Renderer/Renderer.h"

class RenderingBase
{
    friend class RenderingManager;

public:
    virtual ~RenderingBase() = default;

protected:
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void Resize(const uint width, const uint height) {}

    virtual void BeginRender() { }
    virtual void EndRender() { }

    virtual void RenderGeometry() { }
    virtual void RenderShadows() { }
};