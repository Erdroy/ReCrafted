// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "PostProcessingManager.h"

#include "Core/Application.h"
#include "Core/Display.h"
#include "Profiler/Profiler.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/Shader.h"

#include "PostProcessBase.h"

void PostProcessingManager::OnResize(const uint width, const uint height)
{
    DestroyBuffers();
    CreateBuffers(width, height);
}

void PostProcessingManager::CreateBuffers(const uint width, const uint height)
{
    // Create render textures
    m_buffer0 = Renderer::CreateRenderTexture(width, height, Renderer::TextureFormat::RGBA8); // TODO: Use RGBA16 when HDR is done
    m_buffer1 = Renderer::CreateRenderTexture(width, height, Renderer::TextureFormat::RGBA8);
}

void PostProcessingManager::DestroyBuffers()
{
    // Dispose render textures if active
    if (RENDERER_CHECK_HANDLE(m_buffer0))
        Renderer::DestroyTexture2D(m_buffer0);

    if (RENDERER_CHECK_HANDLE(m_buffer1))
        Renderer::DestroyTexture2D(m_buffer1);

    // Cleanup
    m_buffer0 = {};
    m_buffer1 = {};
}

void PostProcessingManager::Initialize()
{
    // Add resize callback
    const auto onResize = Action<void, uint, uint>::New<PostProcessingManager, &PostProcessingManager::OnResize>(this);
    ApplicationWindow::GetDefault()->GetResizeEvent().AddListener(onResize);

    // Create buffers
    CreateBuffers(Display::GetWidth(), Display::GetHeight());
}

void PostProcessingManager::Shutdown()
{
    // Delete all effects
    for(auto&& effect : m_effects)
        delete effect;

    // Clear all effects
    m_effects.Clear();

    // Destroy buffers
    DestroyBuffers();
}

void PostProcessingManager::RenderAllEffects(const Renderer::Texture2DHandle& colorTexture, const Renderer::Texture2DHandle& normalsTexture, const Renderer::Texture2DHandle& depthTexture)
{
    CPU_PROFILE_FUNCTION(0);

    if (m_effects.Empty())
        return;

    auto sourceTexture = colorTexture;
    auto destinationTexture = m_buffer1;

    // Clear the buffers to prevent any garbage from going in. Tho, this is totally optional
    // as this is write-only and blit-only, so never the original contents are preserved.
    Renderer::ClearRenderTexture(m_buffer0, Renderer::Color(0.0f, 0.0f, 0.0f, 1.0f));
    Renderer::ClearRenderTexture(m_buffer1, Renderer::Color(0.0f, 0.0f, 0.0f, 1.0f));

    for (auto i = 0u; i < m_effects.Count(); i++)
    {
        const auto effect = m_effects[i];
        const auto currentShader = effect->GetShader();

        // Set effect's shader as the current one
        RenderingManager::SetCurrentShader(currentShader, true);

        // Render
        effect->Render(currentShader, sourceTexture, normalsTexture, depthTexture, destinationTexture);

        // Reset source texture after the post processing is done
        if (i == 0)
            sourceTexture = m_buffer0;

        // Swap render textures
        std::swap(sourceTexture, destinationTexture);
    }

    // The 'sourceTexture' is now the buffer that contains the latest content

    // Blit 'sourceTexture' to back buffer
    Renderer::BlitTexture(colorTexture, sourceTexture);
}

void PostProcessingManager::AddPostProcess(PostProcessBase* postProcess)
{
    postProcess->OnInitialize();
    m_effects.Add(postProcess);
}
