// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Core/SubSystems/SubSystem.h"
#include "Rendering/DrawMode.h"
#include "Rendering/Renderer/Renderer.h"

/// <summary>
///     RenderingManager class.
/// </summary>
class RenderingManager final : public SubSystem<RenderingManager>
{
    friend class Application;

private:
    RenderingBase* m_rendering = nullptr;

    List<Texture*> m_currentTextures = {};

    Shader* m_currentShader = nullptr;
    DrawMode m_currentDrawMode = DrawMode::Default;

    Renderer::WindowHandle m_windowHandle = {};
    Renderer::RenderBufferHandle m_frameBufferHandle = {};

    List<RenderingComponentBase*> m_renderingComponents = {};

private:
    void InitializeRenderer();

    void OnResize(uint width, uint height);

private: /* Rendering components system */
    template<typename TRenderingComponent>
    void AddRenderingComponent()
    {
        const auto component = TRenderingComponent::GetInstance();
        component->Initialize();
        m_renderingComponents.Add(component);
    }

protected:
    void Initialize() override;
    void Shutdown() override;

    void Render();

    /// <summary>
    ///     Renders all rendering components that are using given stage.
    /// </summary>
    /// <param name="stage">The rendering stage.</param>
    void RenderComponents(RenderingComponentStage stage);

public:
    /// <summary>
    ///     Draws given mesh with currently applied shader.
    /// </summary>
    /// <param name="mesh">The mesh to draw.</param>
    static void DrawIndexedMesh(Mesh* mesh);

    /// <summary>
    ///     Updates the default shader constants.
    /// </summary>
    /// <param name="shader">The shader.</param>
    /// <param name="apply">
    ///     When true, the shader constants will be applied. 
    ///     If false, you will have to call Renderer::ApplyShader to apply it.
    /// </param>
    static void UpdateDefaultConstants(Shader* shader, bool apply = true);

    /// <summary>
    ///     Sets the current draw mode.
    /// </summary>
    /// <param name="drawMode">The new draw mode.</param>
    static void SetDrawMode(DrawMode drawMode);

    /// <summary>
    ///     Sets the current shader.
    /// </summary>
    /// <param name="shader">The shader.</param>
    /// <param name="apply">
    ///     When true, the shader constants will be applied. 
    ///     If false, you will have to call Renderer::ApplyShader to apply it.
    /// </param>
    static void SetCurrentShader(Shader* shader, bool apply = true);

    /// <summary>
    ///     Gets the current shader.
    /// </summary>
    static Shader* GetCurrentShader();

    /// <summary>
    ///     Sets given texture at given slot.
    /// </summary>
    /// <param name="slot">The shader slot.</param>
    /// <param name="texture2d">The texture.</param>
    static void SetTexture(uint slot, Texture* texture2d);

    /// <summary>
    ///     Sets given texture at given slot.
    /// </summary>
    /// <param name="slot">The shader slot.</param>
    /// <param name="textureArray">The textures array.</param>
    /// <param name="textureCount">The texture count.</param>
    /// <remarks>Texture arrays do not support batching, so each call to set texture array will be executed on the GPU.</remarks>
    static void SetTextureArray(uint slot, Texture** textureArray, uint8_t textureCount);

    /// <summary>
    ///     Sets given texture at given slot.
    /// </summary>
    /// <param name="slot">The shader slot.</param>
    /// <param name="textureArray">The textures array.</param>
    /// <param name="textureCount">The texture count.</param>
    /// <remarks>Texture arrays do not support batching, so each call to set texture array will be executed on the GPU.</remarks>
    static void SetTextureHandleArray(uint slot, Renderer::Texture2DHandle* textureArray, uint8_t textureCount);

public:
    /// <summary>
    ///     Gets the current rendering instance.
    /// </summary>
    /// <returns>The rendering instance.</returns>
    static RenderingBase* GetRendering()
    {
        return GetInstance()->m_rendering;
    }

    /// <summary>
    ///     Gets current frame buffer handle.
    /// </summary>
    /// <returns>The current frame buffer handle.</returns>
    static const Renderer::RenderBufferHandle& GetFrameBuffer()
    {
        return GetInstance()->m_frameBufferHandle;
    }

    /// <summary>
    ///     Resizes the current back/front buffers.
    /// </summary>
    /// <param name="width">The new width.</param>
    /// <param name="height">The new height.</param>
    static void Resize(const uint width, const uint height)
    {
        GetInstance()->OnResize(width, height);
    }
};