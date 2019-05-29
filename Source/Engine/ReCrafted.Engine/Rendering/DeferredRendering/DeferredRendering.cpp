// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "DeferredRendering.h"
#include "Common/Logger.h"
#include "Core/Display.h"
#include "Content/ContentManager.h"
#include "Rendering/Shader.h"
#include "Rendering/Camera.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/RenderableBase.h"
#include "Rendering/RenderBuffer.h"
#include "Input/InputManager.h"

void DeferredRendering::Initialize()
{
    Logger::Log("Initializing deferred rendering system");

    Logger::Log("Loading deferred rendering shaders");
    m_gbufferFillShader = ContentManager::LoadAsset<Shader>("Shaders/StandardShader");
    ASSERT(m_gbufferFillShader);

    m_gbufferCombine = ContentManager::LoadAsset<Shader>("Shaders/GBufferCombine");
    ASSERT(m_gbufferCombine);

    // Create render buffer for geometry pass
    Logger::Log("Creating GBuffer");
    m_gbuffer = RenderBuffer::CreateRenderBuffer();
    m_gbuffer->Begin();
    m_gbuffer->AddTarget("ALBEDO", Renderer::TextureFormat::RGBA8);
    m_gbuffer->AddTarget("[RGB]NORMALS, [A]AmbientOcclusion", Renderer::TextureFormat::RGBA8);
    m_gbuffer->AddTarget("[RGB]LIGHT, [A]EMISSION", Renderer::TextureFormat::RGBA8);
    m_gbuffer->End();

    m_frameTexture = Renderer::CreateRenderTexture(Display::GetWidth(), Display::GetHeight(), Renderer::TextureFormat::RGBA8);

    Logger::Log("Deferred rendering initialized");
}

void DeferredRendering::Shutdown()
{
    // Release gbuffer
    m_gbuffer.reset();

    // Destroy render texture
    Renderer::DestroyRenderTexture(m_frameTexture);
}

void DeferredRendering::Resize(const uint width, const uint height)
{
    // Resize gbuffer
    m_gbuffer->Resize(width, height);

    // Resize frame texture
    if (RENDERER_CHECK_HANDLE(m_frameTexture))
    {
        Renderer::DestroyTexture2D(m_frameTexture);
        m_frameTexture = Renderer::CreateRenderTexture(Display::GetWidth(), Display::GetHeight(), Renderer::TextureFormat::RGBA8);
    }
}

void DeferredRendering::BeginRender()
{
    // Set default render stage
    SetDrawMode(DrawMode::Default);

    // Set default shader
    SetShader(m_gbufferFillShader);

    if (InputManager::IsKey(Key::F1))
        Renderer::SetFlag(Renderer::RenderFlags::DrawWireFrame, true);

    // Clean texture cache
    for (auto i = 0u; i < m_currentTextures.Count(); i++)
    {
        m_currentTextures[i] = nullptr;
    }

    // Bind gbuffer
    m_gbuffer->Bind();
}

void DeferredRendering::EndRender()
{
    SetDrawMode(DrawMode::Default);

    if (InputManager::IsKey(Key::F1))
    {
        Renderer::SetFlag(Renderer::RenderFlags::DrawWireFrame, false);
        Renderer::BlitTexture(m_frameTexture, m_gbuffer->GetTarget(0));

        // Reset everything
        m_currentShader = nullptr;
        return;
    }

    if (InputManager::IsKey(Key::F2))
    {
        Renderer::BlitTexture(m_frameTexture, m_gbuffer->GetTarget(0));

        // Reset everything
        m_currentShader = nullptr;
        return;
    }

    if (InputManager::IsKey(Key::F3))
    {
        Renderer::BlitTexture(m_frameTexture, m_gbuffer->GetTarget(1));

        // Reset everything
        m_currentShader = nullptr;
        return;
    }

    if (InputManager::IsKey(Key::F4))
    {
        Renderer::BlitTexture(m_frameTexture, m_gbuffer->GetTarget(2));

        // Reset everything
        m_currentShader = nullptr;
        return;
    }

    if (InputManager::IsKey(Key::F5))
    {
        Renderer::BlitTexture(m_frameTexture, m_gbuffer->GetDepthBuffer());

        // Reset everything
        m_currentShader = nullptr;
        return;
    }

    // Set shader
    SetShader(m_gbufferCombine);

    // Get GBuffer description
    auto& gbufferDescription = Renderer::RenderBufferHandlePool::GetHandleDescription(m_gbuffer->GetHandle());

    // Build render target list (Color, Normal, Depth)
    auto renderTargets = std::vector<Renderer::Texture2DHandle>(gbufferDescription.renderTextures);
    renderTargets.push_back(m_gbuffer->GetDepthBuffer());

    // Blit render textures using gbuffer combine shader
    Renderer::BlitTextures(
        m_frameTexture,
        renderTargets.data(),
        static_cast<uint8_t>(renderTargets.size()),
        m_gbufferCombine->GetHandle()
    );

    // Blit into frame buffer
    Renderer::BlitTexture(RenderingManager::GetFrameBuffer(), m_frameTexture);

    // Reset everything
    m_currentShader = nullptr;
}

void DeferredRendering::RenderGeometry()
{
    //Profiler::BeginProfile("Render Geometry");

    auto cameraFrustum = Camera::GetMainCamera()->GetBoundingFrustum();

    for (auto renderable : RenderingManager::GetGeometryList())
    {
        const auto shader = renderable->GetShader();

        if (m_currentShader != shader)
            SetShader(shader);

        if (!cameraFrustum.Contains(renderable->GetBounds()))
            continue;

        renderable->Render(RenderableRenderMode::RenderGeometry);
    }

    // Cleanup
    SetShader(nullptr);

    //Profiler::EndProfile();
}

void DeferredRendering::RenderShadows()
{
}

void DeferredRendering::UpdateDefaultConstants(Shader* shader) const
{
    const auto mainCamera = Camera::GetMainCamera();

    // Set MVP matrix
    shader->SetValue(0, const_cast<Matrix*>(& mainCamera->GetViewProjection()));

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

void DeferredRendering::SetDrawMode(const DrawMode drawMode)
{
    ASSERT(IS_MAIN_THREAD());
    m_currentDrawMode = drawMode;

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

void DeferredRendering::SetShader(Shader* shader)
{
    m_currentShader = shader;

    if(shader == nullptr)
        return;

    Renderer::ApplyShader(shader->GetHandle(), 0);
    UpdateDefaultConstants(shader);
}

void DeferredRendering::SetTexture(const uint slot, Texture* texture2d)
{
    if (m_currentTextures[slot] != texture2d)
    {
        Renderer::ApplyTexture2D(texture2d->GetHandle(), slot);
        m_currentTextures[slot] = texture2d;
    }
}

void DeferredRendering::SetTextureArray(const uint slot, Texture** textureArray, const uint8_t textureCount)
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

void DeferredRendering::SetTextureHandleArray(const uint slot, Renderer::Texture2DHandle* textureArray, const uint8_t textureCount)
{
    Renderer::ApplyTextureArray2D(textureArray, slot, textureCount);
}
