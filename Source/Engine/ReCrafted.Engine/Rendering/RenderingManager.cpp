// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "RenderingManager.h"
#include "Common/Logger.h"
#include "Core/Display.h"
#include "Rendering/Debug/DebugDraw.h"
#include "Rendering/DeferredRendering/DeferredRendering.h"
#include "Rendering/RenderableBase.h"
#include "Rendering/Camera.h"
#include "Mesh.h"

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
    // Resize rendering's content
    m_rendering->Resize(width, height);

    // Resize window
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
    
    // Pre-Render cleanup
    Renderer::ApplyRenderBuffer(m_frameBufferHandle);
    Renderer::ClearRenderBuffer(m_frameBufferHandle, Renderer::Color(0.0f, 0.0f, 0.0f, 1.0f));

    for (auto i = 0u; i < m_currentTextures.Count(); i++)
        m_currentTextures[i] = nullptr;

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

    // TODO: If HDR is enabled, use tone-mapping.

    // Process rendering components
    RenderComponents(RenderingComponentStage::PostProcess);

    // Set frame buffer as the render target
    Renderer::ApplyRenderBuffer(GetFrameBuffer());

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

void RenderingManager::DrawIndexedMesh(Mesh* mesh)
{
    Renderer::ApplyVertexBuffer(mesh->GetVertexBufferHandle());
    Renderer::ApplyIndexBuffer(mesh->GetIndexBufferHandle());
    Renderer::DrawIndexed(static_cast<uint>(mesh->IndexCount()));
}

void RenderingManager::UpdateDefaultConstants(Shader* shader)
{
    const auto mainCamera = Camera::GetMainCamera();

    // Set MVP matrix
    shader->SetValue(0, const_cast<Matrix*>(&mainCamera->GetViewProjection()));

    // Set inverted view matrix
    auto invView = mainCamera->GetView();
    invView.Invert();
    invView.Transpose();
    shader->SetValue(1, &invView);

    // Set view info
    const auto projection = mainCamera->GetProjection();
    Vector4 viewInfo;
    viewInfo.x = mainCamera->NearPlane();
    viewInfo.y = mainCamera->FarPlane();
    viewInfo.z = 1.0f / projection.m11;
    viewInfo.w = 1.0f / projection.m22;
    shader->SetValue(2, &viewInfo);

    // Set screen size
    Vector2 screenSize;
    screenSize.x = static_cast<float>(Display::GetWidth());
    screenSize.y = static_cast<float>(Display::GetHeight());
    shader->SetValue(3, &screenSize);

    // Set camera position vector
    auto cameraPosition = mainCamera->Position();
    shader->SetValue(4, &cameraPosition);

    // Set light direction vector
    auto lightDir = Vector3(0.2f, 0.3f, 0.1f);
    lightDir.Normalize();
    shader->SetValue(5, &lightDir);

    // Set light direction vector
    auto ambientLight = Vector3(0.35f, 0.35f, 0.35f);
    shader->SetValue(6, &ambientLight);

    // apply shader changes
    Renderer::ApplyShader(shader->GetHandle(), 0);
}

void RenderingManager::SetDrawMode(const DrawMode drawMode)
{
    ASSERT(IS_MAIN_THREAD());
    GetInstance()->m_currentDrawMode = drawMode;

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

void RenderingManager::SetCurrentShader(Shader* shader)
{
    //if (GetInstance()->m_currentShader == shader)
    //    return;

    GetInstance()->m_currentShader = shader;

    if (shader == nullptr)
        return;

    Renderer::ApplyShader(shader->GetHandle(), 0);
    UpdateDefaultConstants(shader);
}

Shader* RenderingManager::GetCurrentShader()
{
    return GetInstance()->m_currentShader;
}

void RenderingManager::SetTexture(const uint slot, Texture * texture2d)
{
    if (GetInstance()->m_currentTextures[slot] != texture2d)
    {
        Renderer::ApplyTexture2D(texture2d->GetHandle(), slot);
        GetInstance()->m_currentTextures[slot] = texture2d;
    }
}

void RenderingManager::SetTextureArray(const uint slot, Texture * *textureArray, const uint8_t textureCount)
{
    // Copy textures
    Renderer::Texture2DHandle textures[32]; // Max 32 textures per array

    for (auto i = 0u; i < textureCount; i++)
    {
        const auto texture = textureArray[i];

        if (texture)
            textures[i] = texture->GetHandle();
    }

    SetTextureHandleArray(slot, textures, textureCount);
}

void RenderingManager::SetTextureHandleArray(const uint slot, Renderer::Texture2DHandle * textureArray, const uint8_t textureCount)
{
    Renderer::ApplyTextureArray2D(textureArray, slot, textureCount);
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
}

void RenderingManager::RemoveRenderable(RenderableBase* renderable)
{
    ASSERT(renderable != nullptr);
    ASSERT(IS_MAIN_THREAD());
    ASSERT(renderable->GetRenderMode() != RenderableRenderMode::None);

    const auto renderMode = renderable->GetRenderMode();

    if ((renderMode & RenderableRenderMode::RenderGeometry) == RenderableRenderMode::RenderGeometry)
        GetInstance()->m_geometryList.Remove(renderable);
}
