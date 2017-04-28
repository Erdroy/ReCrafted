// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Texture2D.h"

#define FREEIMAGE_LIB
#include "FreeImage.h"
#include <bgfx/platform.h>
#include "TextureFormat.h"

#pragma comment(lib, "FreeImageLib.lib")

bool g_freeImage_initialized = false;

static void releaseBits(void* _ptr, void* _userData)
{
	FreeImage_Unload(static_cast<FIBITMAP*>(_userData));
}

void Texture2D::loadFile(const char* filename, uint flags)
{
	auto ver = FreeImage_GetVersion();
	if (!g_freeImage_initialized)
	{

		FreeImage_Initialise();
		g_freeImage_initialized = true;
	}

	auto fif = FreeImage_GetFileType(filename, 0);

	if (fif != FIF_PNG && fif != FIF_JPEG && fif != FIF_DDS && fif != FIF_BMP)
		throw;

	auto bitmap = FreeImage_Load(fif, filename);
	auto bits = FreeImage_GetBits(bitmap);
	auto width = FreeImage_GetWidth(bitmap);
	auto height = FreeImage_GetHeight(bitmap);

	auto size = width * height * 4;

	auto mem = bgfx::makeRef(bits, size, releaseBits, static_cast<void*>(bitmap));
	memcpy_s(mem->data, mem->size, bits, size);

	m_textureHandle = bgfx::createTexture2D(uint16_t(width), uint16_t(height), false, 1, bgfx::TextureFormat::RGBA8, flags, mem);

	// done!

	// FreeImage bitmap will be unloaded with in `releaseBits`
}

void Texture2D::loadMemory(void* data, int size, uint flags)
{

}

uint Texture2D::getPixel(int x, int y)
{
	return 0x0u;
}

void Texture2D::setPixel(int x, int y, uint pixel)
{
}

void Texture2D::apply()
{
	
}

void Texture2D::dispose()
{
	bgfx::destroyTexture(m_textureHandle);
}

Ptr<Texture2D> Texture2D::createTexture()
{
	Ptr<Texture2D> texture(new Texture2D);
	return texture;
}
