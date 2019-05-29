// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "RenderingManager.h"
#include "Common/Logger.h"
#include "Rendering/Debug/DebugDraw.h"
#include "Rendering/DeferredRendering/DeferredRendering.h"
#include "Rendering/RenderableBase.h"
#include "Camera.h"

void RenderingManager::InitializeRenderer()
{
    Logger::Log("Creating renderer with Direct3D11 API");

    Renderer::Initialize(
        Renderer::RendererAPI::DirectX11,
        Renderer::Settings::Debug,
        Renderer::RenderFlags::_enum(Renderer::RenderFlags::Default)
    );

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

    AddRenderingComponent<DebugDraw>();
}

void RenderingManager::Shutdown()
{
    // Release rendering components
    for (auto&& component : m_renderingComponents)
    {
        component->Shutdown();
        component->Release();
        component = nullptr;
    }
    m_renderingComponents.Clear();

    // Release rendering
    m_rendering->Shutdown();
    delete m_rendering;
    m_rendering = nullptr;

    // Shutdown renderer
    Renderer::Shutdown();
}

void RenderingManager::Render()
{
    Renderer::ApplyWindow(m_windowHandle);
    Renderer::ClearRenderBuffer(m_frameBufferHandle, Renderer::Color(0.0f, 0.0f, 0.0f, 1.0f));
    Renderer::ApplyRenderBuffer(m_frameBufferHandle);

    ASSERT(Camera::GetMainCamera());
    Camera::GetMainCamera()->Update();

    // Process rendering components
    RenderComponents(RenderingComponentStage::PreProcess);

    m_rendering->BeginRender(); // Begin rendering
    {
        // Render geometry
        m_rendering->RenderGeometry();
        RenderComponents(RenderingComponentStage::Geometry);

        // Render shadows
        m_rendering->RenderShadows();
    }
    m_rendering->EndRender(); // End rendering

    // TODO: Render post process

    // Process rendering components
    RenderComponents(RenderingComponentStage::PostProcess);

    // Process rendering components
    RenderComponents(RenderingComponentStage::Final);

    Renderer::Frame();
}

void RenderingManager::RenderComponents(const RenderingComponentStage stage)
{
    for (auto& component : m_renderingComponents)
    {
        if (component->GetStage() == stage)
            component->Render();
    }
}

void RenderingManager::SetDrawMode(const DrawMode drawMode)
{
    ASSERT(IS_MAIN_THREAD());
    GetInstance()->m_drawMode = drawMode;

    switch (drawMode)
    {
    case DrawMode::DrawUI:
    case DrawMode::DrawWebUI:
        Renderer::SetFlag(Renderer::RenderFlags::DepthTest, false);
        Renderer::SetFlag(Renderer::RenderFlags::DepthStencil, false);
        Renderer::SetFlag(Renderer::RenderFlags::DrawLineLists, false);
        Renderer::SetFlag(Renderer::RenderFlags::RenderOverlay, true); // temporary simplified blend-states TODO: Expose Renderer's blend state creation
        return;

    case DrawMode::DebugDrawLines:
        Renderer::SetFlag(Renderer::RenderFlags::DepthTest, false);
        Renderer::SetFlag(Renderer::RenderFlags::DepthStencil, false);
        Renderer::SetFlag(Renderer::RenderFlags::DrawLineLists, true);
        Renderer::SetFlag(Renderer::RenderFlags::RenderOverlay, true);
        return;

    case DrawMode::DebugDrawTriangles:
        Renderer::SetFlag(Renderer::RenderFlags::DepthTest, false);
        Renderer::SetFlag(Renderer::RenderFlags::DepthStencil, false);
        Renderer::SetFlag(Renderer::RenderFlags::DrawLineLists, false);
        Renderer::SetFlag(Renderer::RenderFlags::RenderOverlay, true);
        return;

    case DrawMode::Default:
    default:
    {
        Renderer::SetFlags(Renderer::RenderFlags::_enum(Renderer::RenderFlags::Default));
        Renderer::SetFlag(Renderer::RenderFlags::VSync, false);
    }
    }
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

    //if ((renderMode & RenderableRenderMode::RenderShadows) == RenderableRenderMode::RenderShadows)
    //{
    //    ASSERT(GetInstance()->m_shadowGeometryList.Contains(renderable) == false);

    //    GetInstance()->m_shadowGeometryList.Add(renderable);
    //    SortRenderList(GetInstance()->m_shadowGeometryList);
    //}
}

void RenderingManager::RemoveRenderable(RenderableBase* renderable)
{
    ASSERT(renderable != nullptr);
    ASSERT(IS_MAIN_THREAD());
    ASSERT(renderable->GetRenderMode() != RenderableRenderMode::None);

    const auto renderMode = renderable->GetRenderMode();

    if ((renderMode & RenderableRenderMode::RenderGeometry) == RenderableRenderMode::RenderGeometry)
        GetInstance()->m_geometryList.Remove(renderable);

    //if ((renderMode & RenderableRenderMode::RenderShadows) == RenderableRenderMode::RenderShadows)
    //    GetInstance()->m_shadowGeometryList.Remove(renderable);
}
