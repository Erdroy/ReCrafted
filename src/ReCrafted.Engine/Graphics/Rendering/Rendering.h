// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef RENDERING_H
#define RENDERING_H

// includes
#include "ReCrafted.h"
#include "Core/Containers/Array.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderBuffer.h"

class RenderableBase;
class PostProcessingBase;

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
    Array<PostProcessingBase*> m_postProcessingList = {};

    Renderer::Texture2DHandle m_buffer0 = {};
    Renderer::Texture2DHandle m_buffer1 = {};

    Shader* m_lastShader = nullptr;

private:
    static void SortRenderList(const RenderList& list);

private:
    void OnResize(uint width, uint height);

    void CreatePostProcessingBuffers(uint width, uint height);
    void DestroyPostProcessingBuffers();

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
    void RenderPostProcessing(const Renderer::Texture2DHandle& frameTexture, const Renderer::Texture2DHandle& normalsTexture, const Renderer::Texture2DHandle& depthTexture);

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
    static void RegisterPostProcessing(PostProcessingBase* postProcess);

    /**
    * \brief Unregisters post processing effect instance.
    * \param postProcess The post processing effect instance.
    */
    static void UnregisterPostProcessing(PostProcessingBase* postProcess);

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
