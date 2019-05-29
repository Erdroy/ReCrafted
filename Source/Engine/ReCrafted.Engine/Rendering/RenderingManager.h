// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Core/SubSystems/SubSystem.h"
#include "Rendering/Renderer/Renderer.h"

/// <summary>
///     RenderingManager class.
/// </summary>
class RenderingManager final : public SubSystem<RenderingManager>
{
    friend class Application;
    typedef List<RenderableBase*> RenderList;

private:
    RenderList m_geometryList = {};

    RenderingBase* m_rendering = nullptr;

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

private:
    static void SortRenderList(const RenderList& list);

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
    ///     Adds given renderable, to the render list.
    /// </summary>
    /// <param name="renderable">The renderable which will be added.</param>
    static void AddRenderable(RenderableBase* renderable);

    /// <summary>
    ///     Removes given renderable, from the render list.
    /// </summary>
    /// <param name="renderable">The renderable which will be removed.</param>
    static void RemoveRenderable(RenderableBase* renderable);

    /// <summary>
    ///     Gets the amount of geometry renderables in the current list.
    /// </summary>
    /// <returns>The amount of geometry renderables.</returns>
    static int GetRenderableGeometryCount()
    {
        return static_cast<int>(GetInstance()->m_geometryList.Count());
    }

    /// <summary>
    ///     Gets the geometry render list.
    /// </summary>
    /// <returns>The geometry render list.</returns>
    static RenderList& GetGeometryList()
    {
        return GetInstance()->m_geometryList;
    }

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