// ReCrafted (c) 2016-2018 Always Too Late

#include "RenderBuffer.h"
#include "Platform/Platform.h"
#include "Core/Logger.h"

void RenderBuffer::createBuffer(uint width, uint height)
{
	// build render buffer
    m_renderBuffer = Renderer::CreateRenderBuffer(width, height, m_textures, m_textureCount, Renderer::TextureFormat::D32F);
    
    m_width = width;
    m_height = height;
}

void RenderBuffer::begin()
{
	_ASSERT(m_created != true);
}

void RenderBuffer::end()
{
	_ASSERT(m_created != true);

	uint width;
	uint height;
	
	// get window client size
	Platform::getCurrentWindowSize(&width, &height);

	// create framebuffer
	createBuffer(width, height);

	m_created = true;

	Logger::logInfo("Created render buffer");
}

void RenderBuffer::addTarget(const char* name, Renderer::TextureFormat::_enum format)
{
	_ASSERT(m_created != true);

	// add texture
	m_textures[m_textureCount] = format;
	m_textureCount++;
}

Renderer::Texture2DHandle RenderBuffer::getTarget(uint slot)
{
	_ASSERT(slot < MAX_RENDERBUFFER_TARGETS);
	
	return m_renderBuffer.renderTextures[slot];
}

void RenderBuffer::resize(uint width, uint height)
{
	_ASSERT(m_created != false);

    if (m_width == width && m_height == height)
        return;

    // resize RB
    Renderer::ResizeRenderBuffer(m_renderBuffer, width, height);
}

void RenderBuffer::bind()
{
	_ASSERT(m_created != false);

    Renderer::ApplyRenderBuffer(m_renderBuffer);
}

void RenderBuffer::dispose()
{
	// destroy framebuffer
    Renderer::DestroyRenderBuffer(m_renderBuffer);
}
