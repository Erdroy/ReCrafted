// ReCrafted (c) 2016-2019 Always Too Late

#include "RenderBuffer.h"
#include "Platform/Platform.h"
#include "Core/Logger.h"

void RenderBuffer::Begin()
{
    ASSERT(m_created != true);
}

void RenderBuffer::End()
{
    ASSERT(m_created != true);

    uint width;
    uint height;

    // get window client size
    Platform::GetCurrentWindowSize(&width, &height);

    // build render buffer
    m_renderBufferHandle = Renderer::CreateRenderBuffer(width, height, m_textures, m_textureCount,
                                                        Renderer::TextureFormat::D32F);

    m_width = width;
    m_height = height;
    m_created = true;

    Logger::LogInfo("Created render buffer");
}

void RenderBuffer::AddTarget(const char* name, Renderer::TextureFormat::_enum format)
{
    ASSERT(m_created != true);

    // add texture
    m_textures[m_textureCount] = format;
    m_textureCount++;
}

Renderer::Texture2DHandle RenderBuffer::GetTarget(uint slot)
{
    ASSERT(slot < RENDERER_MAX_RENDER_BUFFER_TARGETS);

    rvar renderBufferDesc = Renderer::RenderBufferHandlePool::GetHandleDescription(m_renderBufferHandle);
    return renderBufferDesc.renderTextures[slot];
}

Renderer::Texture2DHandle RenderBuffer::GetDepthBuffer()
{
    rvar renderBufferDesc = Renderer::RenderBufferHandlePool::GetHandleDescription(m_renderBufferHandle);
    ASSERT(RENDERER_CHECK_HANDLE(renderBufferDesc.depthBuffer));

    return renderBufferDesc.depthBuffer;
}

void RenderBuffer::Resize(uint width, uint height)
{
    ASSERT(m_created != false);

    if (m_width == width && m_height == height)
        return;

    // resize RB
    Renderer::ResizeRenderBuffer(m_renderBufferHandle, width, height);
}

void RenderBuffer::Bind()
{
    ASSERT(m_created != false);

    Renderer::ApplyRenderBuffer(m_renderBufferHandle);
}

void RenderBuffer::Dispose()
{
    // Destroy framebuffer
    Renderer::DestroyRenderBuffer(m_renderBufferHandle);
}
