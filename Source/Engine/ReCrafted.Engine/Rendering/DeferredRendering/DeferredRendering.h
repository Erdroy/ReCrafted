// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Rendering/RenderingBase.h"
#include "Rendering/DrawMode.h"
#include "Rendering/Texture.h"

class DeferredRendering final : public RenderingBase
{
    DELETE_COPY_MOVE(DeferredRendering)

private:
    RefPtr<RenderBuffer> m_gbuffer = nullptr;
    Shader* m_gbufferFillShader = nullptr;
    Shader* m_gbufferCombine = nullptr;

    List<Texture*> m_currentTextures = {};

    Renderer::Texture2DHandle m_frameTexture = {};

    Shader* m_currentShader = nullptr;
    DrawMode m_currentDrawMode = DrawMode::Default;

public:
    DeferredRendering() = default;
    ~DeferredRendering() = default;

protected:
    void Initialize() override;
    void Shutdown() override;

    void Resize(uint width, uint height) override;

    void BeginRender() override;
    void EndRender() override;

    void RenderGeometry() override;
    void RenderShadows() override;

public:
    /// <summary>
    ///     Updates the default shader constants.
    /// </summary>
    /// <param name="shader">The shader.</param>
    void UpdateDefaultConstants(Shader* shader) const override;

    /// <summary>
    ///     Sets the current draw mode.
    /// </summary>
    /// <param name="drawMode">The new draw mode.</param>
    void SetDrawMode(DrawMode drawMode) override;

    /// <summary>
    ///     Sets the current shader.
    /// </summary>
    /// <param name="shader">The shader.</param>
    void SetShader(Shader* shader) override;

    /// <summary>
    ///     Sets given texture at given slot.
    /// </summary>
    /// <param name="slot">The shader slot.</param>
    /// <param name="texture2d">The texture.</param>
    void SetTexture(uint slot, Texture* texture2d) override;

    /// <summary>
    ///     Sets given texture at given slot.
    /// </summary>
    /// <param name="slot">The shader slot.</param>
    /// <param name="textureArray">The textures array.</param>
    /// <param name="textureCount">The texture count.</param>
    /// <remarks>Texture arrays do not support batching, so each call to set texture array will be executed on the GPU.</remarks>
    void SetTextureArray(uint slot, Texture** textureArray, uint8_t textureCount) override;

    /// <summary>
    ///     Sets given texture at given slot.
    /// </summary>
    /// <param name="slot">The shader slot.</param>
    /// <param name="textureArray">The textures array.</param>
    /// <param name="textureCount">The texture count.</param>
    /// <remarks>Texture arrays do not support batching, so each call to set texture array will be executed on the GPU.</remarks>
    void SetTextureHandleArray(uint slot, Renderer::Texture2DHandle* textureArray, uint8_t textureCount) override;
};
