// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "DeferredRendering.h"
#include "Common/Logger.h"
#include "Core/Display.h"
#include "Content/ContentManager.h"
#include "Rendering/Shader.h"
#include "Rendering/Camera.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/RenderBuffer.h"
#include "Rendering/PostProcessing/PostProcessingManager.h"
#include "Rendering/PostProcessing/Effects/Vignette.h"
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

    // Register post processing
    PostProcessingManager::AddPostProcess<Vignette>();

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
        Renderer::DestroyRenderTexture(m_frameTexture);
        m_frameTexture = Renderer::CreateRenderTexture(Display::GetWidth(), Display::GetHeight(), Renderer::TextureFormat::RGBA8);
    }
}

void DeferredRendering::BeginRender()
{
    // Set default render stage
    RenderingManager::SetDrawMode(DrawMode::Default);
    
    // Set default shader
    RenderingManager::SetCurrentShader(m_gbufferFillShader);

    if (InputManager::IsKey(Key::F1))
        Renderer::SetFlag(Renderer::RenderFlags::DrawWireFrame, true);

    // Bind gbuffer
    m_gbuffer->Bind();

    const auto clearColor = Renderer::Color{ 0.0f, 0.0f, 0.0f, 1.0f };
    Renderer::ClearRenderBuffer(m_gbuffer->GetHandle(), clearColor);
    Renderer::ClearRenderTexture(m_frameTexture, clearColor);
}

void DeferredRendering::EndRender()
{
    RenderingManager::SetDrawMode(DrawMode::Default);

    if (InputManager::IsKey(Key::F1))
    {
        Renderer::SetFlag(Renderer::RenderFlags::DrawWireFrame, false);
        Renderer::BlitTexture(RenderingManager::GetFrameBuffer(), m_gbuffer->GetTarget(0));

        // Reset everything
        RenderingManager::SetCurrentShader(nullptr);
        return;
    }

    if (InputManager::IsKey(Key::F2))
    {
        Renderer::BlitTexture(RenderingManager::GetFrameBuffer(), m_gbuffer->GetTarget(0));

        // Reset everything
        RenderingManager::SetCurrentShader(nullptr);
        return;
    }

    if (InputManager::IsKey(Key::F3))
    {
        Renderer::BlitTexture(RenderingManager::GetFrameBuffer(), m_gbuffer->GetTarget(1));

        // Reset everything
        RenderingManager::SetCurrentShader(nullptr);
        return;
    }

    if (InputManager::IsKey(Key::F4))
    {
        Renderer::BlitTexture(RenderingManager::GetFrameBuffer(), m_gbuffer->GetTarget(2));

        // Reset everything
        RenderingManager::SetCurrentShader(nullptr);
        return;
    }

    /*if (InputManager::IsKey(Key::F5))
    {
        Renderer::BlitTexture(RenderingManager::GetFrameBuffer(), m_gbuffer->GetDepthBuffer());

        // Reset everything
        RenderingManager::SetCurrentShader(nullptr);
        return;
    }*/

    // Set shader
    RenderingManager::SetCurrentShader(m_gbufferCombine);

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

    // Render post processing
    PostProcessingManager::GetInstance()->RenderAllEffects(m_frameTexture, m_gbuffer->GetTarget(1), m_gbuffer->GetDepthBuffer());

    // Blit into frame buffer
    Renderer::BlitTexture(RenderingManager::GetFrameBuffer(), m_frameTexture);

    // Reset everything
    RenderingManager::SetCurrentShader(nullptr);
}

void DeferredRendering::RenderGeometry()
{
}

void DeferredRendering::RenderShadows()
{
}