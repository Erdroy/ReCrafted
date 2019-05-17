// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "RenderingManager.h"
#include "DeferredRendering/DeferredRendering.h"
#include "Common/Logger.h"
#include "RenderableBase.h"

void RenderingManager::InitializeRenderer()
{
    Logger::Log("Creating renderer with Direct3D11 API");

    Renderer::Initialize(
        Renderer::RendererAPI::DirectX11,
        Renderer::Settings::Debug,
        Renderer::RenderFlags::_enum(Renderer::RenderFlags::Default));

    Renderer::SetFlag(Renderer::RenderFlags::VSync, false);

    // Create Output
    m_windowHandle = Renderer::CreateWindowHandle(Platform::GetCurrentWindow());
    m_frameBufferHandle = Renderer::GetWindowRenderBuffer(m_windowHandle);
}

void RenderingManager::OnResize(const uint width, const uint height)
{
    Renderer::ResizeWindow(m_windowHandle, width, height);
}

void RenderingManager::SortRenderList(const RenderList& list)
{
    // TODO: Sort render list
}

void RenderingManager::Initialize()
{
    // Initialize renderer
    InitializeRenderer();

    m_rendering = new DeferredRendering();
    m_rendering->Initialize();

    // TODO: RenderingComponent system
    // This will be used for ImGui, DebugDraw, WebUIRender etc.
}

void RenderingManager::Shutdown()
{
    m_rendering->Shutdown();
    delete m_rendering;
    m_rendering = nullptr;

    // Shutdown renderer
    Renderer::Shutdown();
}

void RenderingManager::Render()
{
    Renderer::ClearRenderBuffer(m_frameBufferHandle, Renderer::Color(0.0f, 0.0f, 0.0f, 1.0f));
    m_rendering->BeginRender();

    m_rendering->BeginRenderGeometry();
    RenderGeometry();
    m_rendering->EndRenderGeometry();

    //m_rendering->BeginRenderShadows();
    //RenderShadows();
    //m_rendering->EndRenderShadows();

    m_rendering->EndRender();

    // TODO: Render post process
    // TODO: Process RenderingComponent's

    Renderer::Frame();
}

void RenderingManager::RenderGeometry()
{
}

void RenderingManager::AddRenderable(RenderableBase* renderable)
{
    ASSERT(renderable != nullptr);
    ASSERT(IS_MAIN_THREAD());
    ASSERT(renderable->GetRenderMode() != RenderableRenderMode::None);

    const auto renderMode = renderable->GetRenderMode();

    if ((renderMode & RenderableRenderMode::RenderGeometry) == RenderableRenderMode::RenderGeometry)
    {
        ASSERT(GetInstance()->m_geometryList.Contains(renderable) == false);

        GetInstance()->m_geometryList.Add(renderable);
        SortRenderList(GetInstance()->m_geometryList);
    }

    if ((renderMode & RenderableRenderMode::RenderShadows) == RenderableRenderMode::RenderShadows)
    {
        ASSERT(GetInstance()->m_shadowGeometryList.Contains(renderable) == false);

        GetInstance()->m_shadowGeometryList.Add(renderable);
        SortRenderList(GetInstance()->m_shadowGeometryList);
    }
}

void RenderingManager::RemoveRenderable(RenderableBase* renderable)
{
    ASSERT(renderable != nullptr);
    ASSERT(IS_MAIN_THREAD());
    ASSERT(renderable->GetRenderMode() != RenderableRenderMode::None);

    const auto renderMode = renderable->GetRenderMode();

    if ((renderMode & RenderableRenderMode::RenderGeometry) == RenderableRenderMode::RenderGeometry)
        GetInstance()->m_geometryList.Remove(renderable);

    if ((renderMode & RenderableRenderMode::RenderShadows) == RenderableRenderMode::RenderShadows)
        GetInstance()->m_shadowGeometryList.Remove(renderable);
}
