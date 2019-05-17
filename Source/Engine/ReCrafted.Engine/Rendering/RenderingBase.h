// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class RenderingBase
{
    friend class RenderingManager;

public:
    virtual ~RenderingBase() = default;

protected:
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void BeginRender() { }
    virtual void EndRender() { }

    virtual void BeginRenderGeometry() { }
    virtual void EndRenderGeometry() { }
};