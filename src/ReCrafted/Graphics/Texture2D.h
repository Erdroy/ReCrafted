// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

// includes
#include "../Utils/Types.h"
#include <bgfx/bgfx.h>

class Texture2D
{
	friend class Shader;

private:
	bgfx::TextureHandle m_textureHandle = {};

public:
	void loadFile(const char* filename, uint flags = 0u);

	void loadMemory(void* data, int size, uint flags = 0u);

	uint getPixel(int x, int y);

	void setPixel(int x, int y, uint pixel);

	void apply();

	void dispose();

public:
	/// <summary>
	/// Loads texture.
	/// </summary>
	static Ptr<Texture2D> createTexture();
};

#endif // TEXTURE2D_H
