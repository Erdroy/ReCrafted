// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

enum class RenderingComponentStage : byte
{
    /// <summary>
    ///     Provides write access to GBuffer.
    /// </summary>
    Geometry,

    /// <summary>
    ///     Provides write access to frame buffer after it's being cleaned (pre-geometry rendering).
    /// </summary>
    PreProcess,

    /// <summary>
    ///     Provides read access from GBuffer.
    /// </summary>
    PostProcess,

    /// <summary>
    ///     Provides write access to frame buffer.
    /// </summary>
    Final
};

class RenderingComponentBase
{
    friend class RenderingManager;
    DELETE_COPY_MOVE(RenderingComponentBase)

protected:
    RenderingComponentBase() = default;
    virtual ~RenderingComponentBase() = default;

protected:
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void Render() = 0;

protected:
    virtual void Release() = 0;

public:
    virtual RenderingComponentStage GetStage() = 0;
};