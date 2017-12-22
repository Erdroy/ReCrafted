// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

// includes
#include "Core/Types.h"
#include <bgfx/bgfx.h>
#include <Common/ReCraftedAPI.h>
#include <Scripting/Object.h>

typedef struct upng_t upng_t;

class Texture2D : public Object
{
	friend class Shader;
	API_DEF

private:
	bgfx::TextureHandle m_textureHandle = {};

	upng_t* m_bitmap = nullptr;
	byte* m_bits = nullptr;

	uint m_flags = 0u;
	uint m_width = 0u;
	uint m_height = 0u;
	uint m_bpp = 0u;
	uint m_mips = 0u;

public:
	~Texture2D();

public:
	/// <summary>
	/// Load texture from file.
	/// </summary>
	/// <param name="filename">The file.</param>
	/// <param name="flags">BGFX Texture flags.</param>
	void loadFile(const char* filename, uint flags = 0u);

	/// <summary>
	/// Load texture from memory.
	/// </summary>
	/// <param name="data">The texture bits pointer.</param>
	/// <param name="width">The width.</param>
	/// <param name="height">The height.</param>
	/// <param name="flags">BGFX Texture flags.</param>
	void loadMemory(void* data, int width, int height, uint flags = 0u);

	/// <summary>
	/// Create texture memory of given size.
	/// </summary>
	/// <param name="width">The width.</param>
	/// <param name="height">The height.</param>
	/// <param name="flags">BGFX Texture flags.</param>
	void createMemory(int width, int height, uint flags = 0u);

	/// <summary>
	/// Add mipmap. This will reallocate the memory.
	/// </summary>
	/// <param name="width">The width.</param>
	/// <param name="height">The height.</param>
	/// <param name="pixels">The pixels pointer.</param>
	void addMip(int width, int height, uint* pixels);

	/// <summary>
	/// Get pixel at given position.
	/// </summary>
	/// <param name="x">The x coord.</param>
	/// <param name="y">The y coord.</param>
	/// <returns>The pixel HEX.</returns>
	uint getPixel(int x, int y);

	/// <summary>
	/// Get all pixels in the texture(Returns pixel buffer).
	/// This will be invalid after calling `apply`.
	/// </summary>
	/// <returns>The pixels.</returns>
	uint* getPixels() const;

	void setPixel(int x, int y, uint pixel);

	void setPixel(int x, int y, byte r, byte g, byte b, byte a);

	void setPixels(int x, int y, int width, int height, uint* pixels);

	uint getHandle();

	uint getWidth() const;

	uint getHeight() const;

	/// <summary>
	/// Apply, all pixel operations will be unavailable after calling this method.
	/// </summary>
	void apply();

public:
	/// <summary>
	/// Loads texture.
	/// </summary>
	static Ptr<Texture2D> createTexture();
};

#endif // TEXTURE2D_H
