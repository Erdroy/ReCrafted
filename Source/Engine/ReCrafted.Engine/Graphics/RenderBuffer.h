// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Renderer/Renderer.h"

#define RENDERVIEW_BACKBUFFER 0
#define RENDERVIEW_GBUFFER 1

/// <summary>
///     RenderBuffer class.
/// </summary>
class RenderBuffer
{
    friend class Graphics;

private:
    Renderer::RenderBufferHandle m_renderBufferHandle = {};
    Renderer::TextureFormat::_enum m_textures[RENDERER_MAX_RENDER_BUFFER_TARGETS] = {};
    uint m_textureCount = 0u;
    bool m_created = false;

    uint m_width = 0;
    uint m_height = 0;

public:
    RenderBuffer() = default;
    ~RenderBuffer();

public:
    /// <summary>
    ///     Begin render buffer building.
    /// </summary>
    void Begin();

    /// <summary>
    ///     End render buffer building.
    /// </summary>
    void End();

    /// <summary>
    ///     Add texture to render buffer.
    ///     Must be called between begin() and end().
    /// </summary>
    /// <param name="name">The target name, only for better building in code.</param>
    /// <param name="format">The target format.</param>
    void AddTarget(const char* name, Renderer::TextureFormat::_enum format);

    /// <summary>
    ///     Returns texture handle at slot.
    /// </summary>
    /// <param name="slot">The texture/target slot.</param>
    /// <returns>The target texture handle.</returns>
    Renderer::Texture2DHandle GetTarget(uint slot) const;

    /// <summary>
    ///     Returns depth buffer handle.
    /// </summary>
    /// <returns>The depth buffer handle.</returns>
    Renderer::Texture2DHandle GetDepthBuffer() const;

    /// <summary>
    ///     Returns render buffer handle.
    /// </summary>
    /// <returns>The render buffer handle.</returns>
    Renderer::RenderBufferHandle GetHandle() const
    {
        return m_renderBufferHandle;
    }

    /// <summary>
    ///     Resize the render buffer.
    /// </summary>
    /// <param name="width">The width.</param>
    /// <param name="height">The height.</param>
    void Resize(uint width, uint height) const;

    /// <summary>
    ///     Bind this render buffer.
    /// </summary>
    void Bind() const;

public:
    /// <summary>
    ///     Create render buffer.
    /// </summary>
    /// <returns>The created render buffer.</returns>
    FORCEINLINE static RefPtr<RenderBuffer> CreateRenderBuffer()
    {
        RefPtr<RenderBuffer> renderTarget(new RenderBuffer);
        return renderTarget;
    }
};