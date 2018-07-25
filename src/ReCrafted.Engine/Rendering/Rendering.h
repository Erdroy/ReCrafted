// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RENDERING_H
#define RENDERING_H

// includes
#include "ReCrafted.h"
#include "Core/Containers/Array.h"
#include "Graphics/Shader.h"

class RenderableBase;
class PostProcessBase;

typedef Array<RenderableBase*> RenderList;

/**
 * \brief Rendering class.
 */
class Rendering : IDisposable
{
private:
    static Rendering* m_instance;

private:
    RenderList m_geometryList = {};
    RenderList m_shadowGeometryList = {};

    Array<PostProcessBase*> m_postProcessList = {};

    RefPtr<Shader> m_lastShader = nullptr;

private:
    static void SortRenderList(const RenderList& list);

public:
    /**
     * \brief Initializes the Rendering class.
     */
    void Initialize();

    /**
     * \brief Disposes the Rendering class.
     */
    void Dispose() override;

public:
    /**
     * \brief Renders geometry.
     */
    void RenderGeometry();

    /**
     * \brief Renders raw geometry for shadow rendering.
     */
    void RenderShadows();

    /**
     * \brief Renders all post processing effects.
     */
    void RenderPostProcessing();

public:
    /**
     * \brief Adds given renderable, to the render list.
     * \param renderable The renderable which will be added.
     */
    static void AddRenderable(RenderableBase* renderable);

    /**
    * \brief Removes given renderable, from the render list.
    * \param renderable The renderable which will be removed.
    */
    static void RemoveRenderable(RenderableBase* renderable);

    /**
     * \brief Registers post processing effect instance.
     * \param postProcess The post processing effect instance.
     */
    static void RegisterPostProcessing(PostProcessBase* postProcess);

    /**
    * \brief Unregisters post processing effect instance.
    * \param postProcess The post processing effect instance.
    */
    static void UnregisterPostProcessing(PostProcessBase* postProcess);

    /**
     * \brief Gets the amount of geometry renderables in the current list.
     * \return The amount of geometry renderables.
     */
    static int GetRenderableGeometryCount()
    {
        return m_instance->m_geometryList.Count();
    }

    /**
    * \brief Gets the amount of shadow geometry renderables in the current list.
    * \return The amount of shadow geometry renderables.
    */
    static int GetRenderableShadowGeometryCount()
    {
        return m_instance->m_shadowGeometryList.Count();
    }
};

#endif // RENDERING_H
