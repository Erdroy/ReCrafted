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
    RenderList m_shadowGeometryList = {};

    RenderingBase* m_rendering = nullptr;

    Renderer::WindowHandle m_windowHandle = {};
    Renderer::RenderBufferHandle m_frameBufferHandle = {};

private:
    void InitializeRenderer();

    void OnResize(uint width, uint height);

private:
    static void SortRenderList(const RenderList& list);

protected:
    void Initialize() override;
    void Shutdown() override;

    void Render();

    void RenderGeometry();

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
    ///     Gets the amount of shadow geometry renderables in the current list.
    /// </summary>
    /// <returns>The amount of shadow geometry renderables.</returns>
    static int GetRenderableShadowGeometryCount()
    {
        return static_cast<int>(GetInstance()->m_shadowGeometryList.Count());
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