// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "RenderingManager.h"
#include "Common/Logger.h"
#include "Core/Display.h"
#include "Rendering/Debug/DebugDraw.h"
#include "Rendering/ImGui/ImGuiManager.h"
#include "Rendering/DeferredRendering/DeferredRendering.h"
#include "Rendering/Models/ModelRenderingSystem.h"
#include "Rendering/Camera.h"
#include "Rendering/Mesh.h"
#include "Profiler/Profiler.h"
#include "WebUI/WebUIManager.h"

void RenderingManager::InitializeRenderer()
{
    Logger::Log("Creating renderer with Direct3D11 API");

    Renderer::Initialize(
        Renderer::RendererAPI::DirectX11,
        Renderer::Settings::Debug,
        Renderer::RenderFlags::_enum(Renderer::RenderFlags::Default)
    );

    Renderer::SetFlag(Renderer::RenderFlags::VSync, true);

    // Create Output
    m_windowHandle = Renderer::CreateWindowHandle(Platform::GetCurrentWindow());
    m_frameBufferHandle = Renderer::GetWindowRenderBuffer(m_windowHandle);
}

void RenderingManager::OnResize(const uint width, const uint height)
{
    // Resize rendering's content
    m_rendering->Resize(width, height);

    WebUIManager::GetInstance()->ResizeFullscreenViews(width, height);

    // Resize window
    Renderer::ResizeWindow(m_windowHandle, width, height);
}

void RenderingManager::Initialize()
{
    // Initialize renderer
    InitializeRenderer();

    m_rendering = new DeferredRendering();
    m_rendering->Initialize();

    AddRenderingComponent<ModelRenderingSystem>();
    AddRenderingComponent<DebugDraw>();
    AddRenderingComponent<ImGuiManager>();

    Renderer::AddOnPresentBeginEvent(Action<void>::New<ImGuiManager, &ImGuiManager::EndRender>(ImGuiManager::GetInstance()));
    Renderer::AddOnPresentBeginEvent(Action<void>::New<WebUIManager, &WebUIManager::Render>(WebUIManager::GetInstance()));
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
    m_renderingComponents.Release();

    // Release rendering
    m_rendering->Shutdown();
    delete m_rendering;
    m_rendering = nullptr;

    // Shutdown renderer
    Renderer::Shutdown();
}

void RenderingManager::EarlyUpdate()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    ImGuiManager::GetInstance()->BeginRender();
}

void RenderingManager::Render()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

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

    // Render WebUI
    SetDrawMode(DrawMode::DrawWebUI);
    WebUIManager::GetInstance()->RenderViews();

    // Process rendering components
    RenderComponents(RenderingComponentStage::Final);

    Profiler::BeginCPUProfile("Renderer::Frame");
    Renderer::Frame();
    Profiler::EndCPUProfile();
}

void RenderingManager::RenderComponents(const RenderingComponentStage stage)
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

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

void RenderingManager::UpdateDefaultConstants(Shader* shader, bool apply)
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

    if(apply)
    {
        // apply shader changes
        Renderer::ApplyShader(shader->GetHandle(), 0);
    }
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
        // Store vSync state to restore it later (we don't want to change it right there).
        const auto vSyncEnabled = Renderer::GetFlag(Renderer::RenderFlags::VSync);

        Renderer::SetFlags(Renderer::RenderFlags::_enum(Renderer::RenderFlags::Default));

        // Restore VSync state
        Renderer::SetFlag(Renderer::RenderFlags::VSync, vSyncEnabled);
    }
    }
}

void RenderingManager::SetCurrentShader(Shader* shader, bool apply)
{
    //if (GetInstance()->m_currentShader == shader)
    //    return;

    GetInstance()->m_currentShader = shader;

    if (shader == nullptr)
        return;

    //Renderer::ApplyShader(shader->GetHandle(), 0);
    UpdateDefaultConstants(shader, apply);
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
