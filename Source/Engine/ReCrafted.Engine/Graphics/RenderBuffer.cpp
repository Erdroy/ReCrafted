// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "RenderBuffer.h"
#include "Common/Logger.h"

RenderBuffer::~RenderBuffer()
{
    // Destroy framebuffer
    Renderer::DestroyRenderBuffer(m_renderBufferHandle);
}

void RenderBuffer::Begin()
{
    ASSERT(m_created != true);
}

void RenderBuffer::End()
{
    ASSERT(m_created != true);

    uint width;
    uint height;

    // Get window client size
    Platform::GetCurrentWindowSize(&width, &height);

    // Build render buffer
    m_renderBufferHandle = Renderer::CreateRenderBuffer(width, height, m_textures, m_textureCount,
        Renderer::TextureFormat::D32F);

    m_width = width;
    m_height = height;
    m_created = true;

    Logger::Log("Created render buffer");
}

void RenderBuffer::AddTarget(const char* name, const Renderer::TextureFormat::_enum format)
{
    ASSERT(m_created != true);

    // Add texture
    m_textures[m_textureCount] = format;
    m_textureCount++;
}

Renderer::Texture2DHandle RenderBuffer::GetTarget(const uint slot) const
{
    ASSERT(slot < RENDERER_MAX_RENDER_BUFFER_TARGETS);

    auto& renderBufferDesc = Renderer::RenderBufferHandlePool::GetHandleDescription(m_renderBufferHandle);
    return renderBufferDesc.renderTextures[slot];
}

Renderer::Texture2DHandle RenderBuffer::GetDepthBuffer() const
{
    auto& renderBufferDesc = Renderer::RenderBufferHandlePool::GetHandleDescription(m_renderBufferHandle);
    ASSERT(RENDERER_CHECK_HANDLE(renderBufferDesc.depthBuffer));

    return renderBufferDesc.depthBuffer;
}

void RenderBuffer::Resize(const uint width, const uint height) const
{
    ASSERT(m_created != false);

    if (m_width == width && m_height == height)
        return;

    // Resize RB
    Renderer::ResizeRenderBuffer(m_renderBufferHandle, width, height);
}

void RenderBuffer::Bind() const
{
    ASSERT(m_created != false);

    Renderer::ApplyRenderBuffer(m_renderBufferHandle);
}
