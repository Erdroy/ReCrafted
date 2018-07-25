// ReCrafted (c) 2016-2018 Always Too Late

#include "Rendering.h"
#include "RenderableBase.h"
#include "Common/Profiler/Profiler.h"
#include "Graphics/Graphics.h"
#include "Graphics/Camera.h"

Rendering* Rendering::m_instance;

void Rendering::SortRenderList(const RenderList& list)
{
    // TODO: Sort render list
}

void Rendering::Initialize()
{
    m_instance = this;

}

void Rendering::Dispose()
{
}

void Rendering::RenderGeometry()
{
    Profiler::BeginProfile("Render Geometry");

    var cameraFrustum = Camera::GetMainCamera()->GetBoundingFrustum();

    for (var renderable : m_geometryList)
    {
        if(m_lastShader != renderable->GetShader())
        {
            m_lastShader = renderable->GetShader();
            Graphics::GetInstance()->SetShader(m_lastShader);
        }

        if (!cameraFrustum.Contains(renderable->GetBounds()))
            continue;

        renderable->Render(RenderableRenderMode::RenderGeometry);
    }

    // Cleanup
    m_lastShader = nullptr;

    Profiler::EndProfile();
}

void Rendering::RenderShadows()
{
    Profiler::BeginProfile("Render Shadows");

    for (var renderable : m_shadowGeometryList)
        renderable->Render(RenderableRenderMode::RenderShadows);

    Profiler::EndProfile();
}

void Rendering::RenderPostProcessing()
{
    //Profiler::BeginProfile("Render PostProcess");

    // TODO: Implement post processing

    //Profiler::EndProfile();
}

void Rendering::AddRenderable(RenderableBase* renderable)
{
    ASSERT(renderable != nullptr);
    ASSERT(IS_MAIN_THREAD());
    ASSERT(renderable->GetRenderMode() != RenderableRenderMode::None);

    cvar renderMode = renderable->GetRenderMode();

    if ((renderMode & RenderableRenderMode::RenderGeometry) == RenderableRenderMode::RenderGeometry)
    {
        ASSERT(m_instance->m_geometryList.Contains(renderable) == false);

        m_instance->m_geometryList.Add(renderable);
        SortRenderList(m_instance->m_geometryList);
    }

    if ((renderMode & RenderableRenderMode::RenderShadows) == RenderableRenderMode::RenderShadows)
    {
        ASSERT(m_instance->m_shadowGeometryList.Contains(renderable) == false);

        m_instance->m_shadowGeometryList.Add(renderable);
        SortRenderList(m_instance->m_shadowGeometryList);
    }
}

void Rendering::RemoveRenderable(RenderableBase* renderable)
{
    ASSERT(renderable != nullptr);
    ASSERT(IS_MAIN_THREAD());
    ASSERT(renderable->GetRenderMode() != RenderableRenderMode::None);

    cvar renderMode = renderable->GetRenderMode();

    if ((renderMode & RenderableRenderMode::RenderGeometry) == RenderableRenderMode::RenderGeometry)
        m_instance->m_geometryList.Remove(renderable);

    if ((renderMode & RenderableRenderMode::RenderShadows) == RenderableRenderMode::RenderShadows)
        m_instance->m_shadowGeometryList.Remove(renderable);
}

void Rendering::RegisterPostProcessing(PostProcessBase* postProcess)
{
    ASSERT(postProcess != nullptr);
    ASSERT(IS_MAIN_THREAD());

    m_instance->m_postProcessList.Add(postProcess);
}

void Rendering::UnregisterPostProcessing(PostProcessBase* postProcess)
{
    ASSERT(postProcess != nullptr);
    ASSERT(IS_MAIN_THREAD());

    m_instance->m_postProcessList.Remove(postProcess);
}
