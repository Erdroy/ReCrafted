// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RENDERABLEBASE_H
#define RENDERABLEBASE_H

// includes
#include "ReCrafted.h"
#include "Graphics/Shader.h"
#include "Core/Math/BoundingBox.h"

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
    virtual void Render(RenderableRenderMode renderMode) = 0;
    
    virtual Shader* GetShader() const = 0;
    virtual Vector3& GetPosition() = 0;
    virtual BoundingBox& GetBounds() = 0;

    virtual RenderableRenderMode GetRenderMode()
    {
        return RenderableRenderMode::RenderGeometry | RenderableRenderMode::RenderShadows;
    }
};

#endif // RENDERABLEBASE_H
