// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "RenderBuffer.h"
#include "../Platform/Platform.Windows.h"
#include "../Core/Logger.h"

void RenderBuffer::createBuffer(uint width, uint height)
{
	// sampler flags
	const auto samplerFlags = 0
		| BGFX_TEXTURE_RT
		| BGFX_TEXTURE_MIN_POINT
		| BGFX_TEXTURE_MAG_POINT
		| BGFX_TEXTURE_MIP_POINT
		| BGFX_TEXTURE_U_CLAMP
		| BGFX_TEXTURE_V_CLAMP;

	// create textures
	for (auto i = 0u; i < m_textureCount; i++)
		m_textureHandles[i] = bgfx::createTexture2D(uint16(width), uint16(height), false, 1, static_cast<bgfx::TextureFormat::Enum>(m_textures[i]), samplerFlags);

	// build render buffer
	m_framebufferHandle = bgfx::createFrameBuffer(m_textureCount, m_textureHandles, true);
}

void RenderBuffer::begin()
{
	_ASSERT(m_created != true);

	// zeroe all textures
	for (auto i = 0; i < RENDERBUFFER_MAXTARGETS; i++)
		m_textures[i] = {};
}

void RenderBuffer::end()
{
	_ASSERT(m_created != true);

	uint width;
	uint height;
	
	// get window client size
	Platform::getGameWindowSize(&width, &height);

	// create framebuffer
	createBuffer(width, height);

	m_created = true;

	Logger::write("Created render buffer", LogLevel::Info);
}

void RenderBuffer::addTarget(const char* name, TextureFormat::Enum format)
{
	_ASSERT(m_created != true);

	// add texture
	m_textures[m_textureCount] = format;
	m_textureCount++;
}

bgfx::TextureHandle RenderBuffer::getTarget(uint slot)
{
	_ASSERT(slot < RENDERBUFFER_MAXTARGETS);
	_ASSERT(m_textureHandles[slot].idx > 0);

	return m_textureHandles[slot];
}

void RenderBuffer::resize(uint width, uint height)
{
	_ASSERT(m_created != false);

	// destroy current framebuffer
	bgfx::destroyFrameBuffer(m_framebufferHandle);
	
	// create new framebuffer
	createBuffer(width, height);
}

void RenderBuffer::bind()
{
	_ASSERT(m_created != false);

	// TODO: bind framebuffer
	bgfx::setViewFrameBuffer(RENDERVIEW_GBUFFER, m_framebufferHandle);
}

void RenderBuffer::dispose()
{
	// destroy framebuffer
	bgfx::destroyFrameBuffer(m_framebufferHandle);
	Logger::write("Unloaded render buffer", LogLevel::Info);
}
