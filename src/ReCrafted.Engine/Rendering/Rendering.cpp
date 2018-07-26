// ReCrafted (c) 2016-2018 Always Too Late

#include "Rendering.h"
#include "RenderableBase.h"
#include "Common/Profiler/Profiler.h"
#include "Graphics/Graphics.h"
#include "Graphics/Camera.h"
#include "PostProcessing/PostProcessingBase.h"
#include "Core/EngineMain.h"
#include "Common/Display.h"

Rendering* Rendering::m_instance;

void Rendering::SortRenderList(const RenderList& list)
{
    // TODO: Sort render list
}

void Rendering::OnResize(uint width, uint height)
{
    DestroyPostProcessingBuffers();
    CreatePostProcessingBuffers(width, height);
}

void Rendering::CreatePostProcessingBuffers(uint width, uint height)
{
    // Create render textures
    m_buffer0 = Renderer::CreateTexture2D(width, height, 0, Renderer::TextureFormat::RGBA8, nullptr, 0u, true);
    m_buffer1 = Renderer::CreateTexture2D(width, height, 0, Renderer::TextureFormat::RGBA8, nullptr, 0u, true);
}

void Rendering::DestroyPostProcessingBuffers()
{
    // Dispose render textures
    if (RENDERER_CHECK_HANDLE(m_buffer0))
        Renderer::DestroyTexture2D(m_buffer0);

    if (RENDERER_CHECK_HANDLE(m_buffer1))
        Renderer::DestroyTexture2D(m_buffer1);
}

void Rendering::Initialize()
{
    m_instance = this;

    // Add resize callback
    cvar onResize = Action<void, uint, uint>::New<Rendering, &Rendering::OnResize>(this);
    EngineMain::GetInstance()->OnMainWindowResize.AddListener(onResize);

    // Create buffers
    CreatePostProcessingBuffers(Display::GetWidth(), Display::GetHeight());
}

void Rendering::Dispose()
{
    DestroyPostProcessingBuffers();
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

void Rendering::RenderPostProcessing(const Renderer::RenderBufferHandle& renderBuffer, const Renderer::Texture2DHandle& depthTexture)
{
    Profiler::BeginProfile("Render PostProcessing");

    var sourceTexture = m_buffer0;
    var destinationTexture = m_buffer1;

    for(var i = 0u; i < m_postProcessingList.Count(); i ++)
    {
        cvar currentPP = m_postProcessingList[i];
        cvar currentPPShader = currentPP->GetShader();

        // Render
        currentPP->Render();

        // Blit
        Renderer::BlitTexture(destinationTexture, sourceTexture, currentPPShader->GetHandle());

        // Reset source texture after the post processing is done
        if(i == 0)
            sourceTexture = m_buffer0;

        // Swap render textures
        std::swap(sourceTexture, destinationTexture);
    }

    // The 'destinationTexture' is now the buffer that contains the latest content

    // Blit 'destinationTexture' to back buffer
    Renderer::BlitTexture(renderBuffer, destinationTexture);

    Profiler::EndProfile();
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

void Rendering::RegisterPostProcessing(PostProcessingBase* postProcess)
{
    ASSERT(postProcess != nullptr);
    ASSERT(IS_MAIN_THREAD());

    postProcess->OnInitialize();
    m_instance->m_postProcessingList.Add(postProcess);
}

void Rendering::UnregisterPostProcessing(PostProcessingBase* postProcess)
{
    ASSERT(postProcess != nullptr);
    ASSERT(IS_MAIN_THREAD());

    m_instance->m_postProcessingList.Remove(postProcess);
}
