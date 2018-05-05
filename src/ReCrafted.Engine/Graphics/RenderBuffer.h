// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

// includes
#include "ReCrafted.h"
#include "TextureFormat.h"

#define RENDERVIEW_BACKBUFFER 0
#define RENDERVIEW_GBUFFER 1

/// <summary>
/// RenderBuffer class.
/// </summary>
class RenderBuffer
{
private:
	//bgfx::FrameBufferHandle m_framebufferHandle = {};
	TextureFormat::Enum m_textures[MAX_RENDERBUFFER_TARGETS] = {};
	//bgfx::TextureHandle m_textureHandles[MAX_RENDERBUFFER_TARGETS] = {};
	uint m_textureCount = 0u;
	bool m_created = false;

    uint m_width = 0;
    uint m_height = 0;

private:
	
	void createBuffer(uint width, uint height);

public:
	/// <summary>
	/// Begin render buffer building.
	/// </summary>
	void begin();

	/// <summary>
	/// End render buffer building.
	/// </summary>
	void end();

	/// <summary>
	/// Add texture to render buffer.
	/// Must be called between begin() and end().
	/// </summary>
	/// <param name="name">The target name, only for better building in code.</param>
	/// <param name="format">The target format.</param>
	void addTarget(const char* name, TextureFormat::Enum format);

	/// <summary>
	/// Returns texture handle at slot.
	/// </summary>
	/// <param name="slot">The texture/target slot.</param>
	/// <returns>The target texture handle.</returns>
	//bgfx::TextureHandle getTarget(uint slot);

	/// <summary>
	/// Resize the render buffer.
	/// </summary>
	/// <param name="width">The width.</param>
	/// <param name="height">The height.</param>
	void resize(uint width, uint height);

	/// <summary>
	/// Bind this render buffer.
	/// </summary>
	void bind();

	/// <summary>
	/// Dispose this render buffer.
	/// </summary>
	void dispose();

public:
	/// <summary>
	/// Create render buffer.
	/// </summary>
	/// <returns>The created render buffer.</returns>
	FORCEINLINE static Ref<RenderBuffer> createRenderTarget()
	{
		Ref<RenderBuffer> renderTarget(new RenderBuffer);
		return renderTarget;
	}
};

#endif // RENDERBUFFER_H