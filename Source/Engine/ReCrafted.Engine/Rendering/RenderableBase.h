// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

enum class RenderableRenderMode : uint32_t
{
    None = 0,
    RenderGeometry = 1 << 0,
    RenderShadows = 1 << 1
};
DEFINE_ENUM(RenderableRenderMode)

class RenderableBase
{
public:
    virtual ~RenderableBase() = default;

public:
    virtual Mesh* GetMesh() const = 0;
    virtual Shader* GetShader() const = 0;
    virtual Vector3& GetPosition() = 0;
    virtual BoundingBox& GetBounds() = 0;

    virtual RenderableRenderMode GetRenderMode()
    {
        return RenderableRenderMode::RenderGeometry | RenderableRenderMode::RenderShadows;
    }

    // TODO: Model and Material class.
};
