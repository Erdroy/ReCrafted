// ReCrafted (c) 2016-2018 Always Too Late

#include "RenderBuffer.h"
#include "Platform/Platform.h"
#include "Core/Logger.h"

void RenderBuffer::Begin()
{
    _ASSERT(m_created != true);
}

void RenderBuffer::End()
{
    _ASSERT(m_created != true);

    uint width;
    uint height;

    // get window client size
    Platform::GetCurrentWindowSize(&width, &height);

    // build render buffer
    m_renderBufferHandle = Renderer::CreateRenderBuffer(width, height, m_textures, m_textureCount,
                                                        Renderer::TextureFormat::D32);

    m_width = width;
    m_height = height;
    m_created = true;

    Logger::LogInfo("Created render buffer");
}

void RenderBuffer::AddTarget(const char* name, Renderer::TextureFormat::_enum format)
{
    _ASSERT(m_created != true);

    // add texture
    m_textures[m_textureCount] = format;
    m_textureCount++;
}

Renderer::Texture2DHandle RenderBuffer::GetTarget(uint slot)
{
    _ASSERT(slot < RENDERER_MAX_RENDER_BUFFER_TARGETS);

    var renderBufferDesc = Renderer::GetRenderBufferDescription(m_renderBufferHandle);
    return renderBufferDesc.renderTextures[slot];
}

Renderer::Texture2DHandle RenderBuffer::GetDepthBuffer()
{
    var renderBufferDesc = Renderer::GetRenderBufferDescription(m_renderBufferHandle);
    ASSERT(RENDERER_CHECK_HANDLE(renderBufferDesc.depthBuffer));

    return renderBufferDesc.depthBuffer;
}

void RenderBuffer::Resize(uint width, uint height)
{
    _ASSERT(m_created != false);

    if (m_width == width && m_height == height)
        return;

    // resize RB
    Renderer::ResizeRenderBuffer(m_renderBufferHandle, width, height);
}

void RenderBuffer::Bind()
{
    _ASSERT(m_created != false);

    Renderer::ApplyRenderBuffer(m_renderBufferHandle);
}

void RenderBuffer::Dispose()
{
    // Destroy framebuffer
    Renderer::DestroyRenderBuffer(m_renderBufferHandle);
}
