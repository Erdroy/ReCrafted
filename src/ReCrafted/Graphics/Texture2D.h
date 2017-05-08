// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

// includes
#include "../Utils/Types.h"
#include <bgfx/bgfx.h>

struct FIBITMAP;

class Texture2D
{
	friend class Shader;

private:
	bgfx::TextureHandle m_textureHandle = {};

	FIBITMAP* m_bitmap = nullptr;
	byte* m_bits = nullptr;

	uint m_flags = 0u;
	uint m_width = 0u;
	uint m_height = 0u;

public:
	void loadFile(const char* filename, uint flags = 0u);

	void loadMemory(void* data, int width, int height, uint flags = 0u);

	void createMemory(int width, int height, uint flags = 0u);

	uint getPixel(int x, int y);

	void setPixel(int x, int y, uint pixel);

	void setPixel(int x, int y, byte r, byte g, byte b, byte a);

	void setPixels(int x, int y, int width, int height, uint* pixels);

	void apply(bool readable = true);

	void dispose();

public:
	/// <summary>
	/// Loads texture.
	/// </summary>
	static Ptr<Texture2D> createTexture();

public:
	static void loadTextureData(const char* filename, uint** pixels, int* width, int* height);
	static void releaseTextureData(uint* pixels);
};

#endif // TEXTURE2D_H
