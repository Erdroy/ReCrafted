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

public:
    /// <summary>
    ///     Updates the default shader constants.
    /// </summary>
    /// <param name="shader">The shader.</param>
    virtual void UpdateDefaultConstants(Shader* shader) const {}

    /// <summary>
    ///     Sets the current draw mode.
    /// </summary>
    /// <param name="drawMode">The new draw mode.</param>
    virtual void SetDrawMode(DrawMode drawMode) {}

    /// <summary>
    ///     Sets the current shader.
    /// </summary>
    /// <param name="shader">The shader.</param>
    virtual void SetShader(Shader* shader) {}

    /// <summary>
    ///     Sets given texture at given slot.
    /// </summary>
    /// <param name="slot">The shader slot.</param>
    /// <param name="texture2d">The texture.</param>
    virtual void SetTexture(uint slot, Texture* texture2d) {}   
    
    /// <summary>
    ///     Sets given texture at given slot.
    /// </summary>
    /// <param name="slot">The shader slot.</param>
    /// <param name="textureArray">The textures array.</param>
    /// <param name="textureCount">The texture count.</param>
    /// <remarks>Texture arrays do not support batching, so each call to set texture array will be executed on the GPU.</remarks>
    virtual void SetTextureArray(uint slot, Texture** textureArray, uint8_t textureCount) {}    
    
    /// <summary>
    ///     Sets given texture at given slot.
    /// </summary>
    /// <param name="slot">The shader slot.</param>
    /// <param name="textureArray">The textures array.</param>
    /// <param name="textureCount">The texture count.</param>
    /// <remarks>Texture arrays do not support batching, so each call to set texture array will be executed on the GPU
    virtual void SetTextureHandleArray(uint slot, Renderer::Texture2DHandle* textureArray, uint8_t textureCount) {}
};