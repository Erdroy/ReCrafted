// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "RenderBuffer.h"
#include "../Platform/Platform.Windows.h"

void RenderBuffer::createBuffer(uint width, uint height)
{
	static bgfx::TextureHandle textures[RENDERBUFFER_MAXTARGETS];

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
		textures[i] = bgfx::createTexture2D(uint16(width), uint16(height), false, 1, static_cast<bgfx::TextureFormat::Enum>(m_textures[i]), samplerFlags);

	// build render buffer
	m_framebufferHandle = bgfx::createFrameBuffer(m_textureCount, textures, true);
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
}

void RenderBuffer::addTarget(const char* name, TextureFormat::Enum format)
{
	_ASSERT(m_created != true);

	// add texture
	m_textures[m_textureCount] = format;
	m_textureCount++;
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
	bgfx::setViewFrameBuffer(0, m_framebufferHandle);
}

void RenderBuffer::dispose()
{
	// destroy framebuffer
	bgfx::destroyFrameBuffer(m_framebufferHandle);
}
