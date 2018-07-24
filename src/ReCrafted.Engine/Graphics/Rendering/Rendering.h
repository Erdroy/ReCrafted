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

class Rendering : IDisposable
{
private:
    static Rendering* m_instance;

private:
    RenderList m_geometryList = {};
    RenderList m_shadowList = {};

    Array<PostProcessBase*> m_postProcessList = {};

    RefPtr<Shader> m_lastShader = nullptr;

private:
    static void SortRenderList(const RenderList& list);

public:
    void Initialize();
    void Dispose() override;

public:
    void RenderGeometry();
    void RenderShadows();
    void RenderPostProcessing();

public:
    static void AddRenderable(RenderableBase* renderable);
    static void RemoveRenderable(RenderableBase* renderable);

    static void RegisterPostProcessing(PostProcessBase* postProcess);
};

#endif // RENDERING_H
