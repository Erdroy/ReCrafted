// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Texture2D.h"

#define FREEIMAGE_LIB
#include "FreeImage.h"
#include "TextureFormat.h"

#pragma comment(lib, "FreeImageLib.lib")

bool g_freeImage_initialized = false;

static void releaseBits(void* _ptr, void* _userData)
{
	FreeImage_Unload(static_cast<FIBITMAP*>(_userData));
}

static void releaseBitsRaw(void* _ptr, void* _userData)
{
	delete [] _userData;
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

	m_bitmap = FreeImage_Load(fif, filename);
	m_bits = FreeImage_GetBits(m_bitmap);
	m_width = FreeImage_GetWidth(m_bitmap);
	m_height = FreeImage_GetHeight(m_bitmap);
	m_flags = flags;
	
	// done!

	// FreeImage bitmap will be unloaded with in `releaseBits`
}

void Texture2D::loadMemory(void* data, int width, int height, uint flags)
{
	m_width = width;
	m_height = height;

	m_bits = new byte[width * height * 4];

	if(data)
		memcpy(m_bits, data, width * height * 4);

	// done

	// bits will be unloaded with in `releaseBitsRaw`
}

void Texture2D::createMemory(int width, int height, uint flags)
{
	loadMemory(nullptr, width, height, flags);
}

uint Texture2D::getPixel(int x, int y)
{
	if (m_bits == nullptr)
		return 0x0u;

	auto idx = y * m_width + x;

	auto r = m_bits[idx + 0];
	auto g = m_bits[idx + 1];
	auto b = m_bits[idx + 2];
	auto a = m_bits[idx + 3];

	return a | b << 8 | g << 16 | r << 24;
}

void Texture2D::setPixels(int x, int y, int width, int height, uint* pixels)
{
	MISSING_CODE();
}

void Texture2D::setPixel(int x, int y, uint pixel)
{
	if (m_bits == nullptr)
		return;

	auto r = (pixel & 0xFF000000) >> 24;
	auto g = (pixel & 0x00FF0000) >> 16;
	auto b = (pixel & 0x0000FF00) >> 8;
	auto a = (pixel & 0x000000FF);

	auto idx = y * m_width + x;

	m_bits[idx + 0] = r;
	m_bits[idx + 1] = g;
	m_bits[idx + 2] = b;
	m_bits[idx + 3] = a;
}

void Texture2D::setPixel(int x, int y, byte r, byte g, byte b, byte a)
{
	if (m_bits == nullptr)
		return;

	auto idx = y * m_width + x;

	m_bits[idx + 0] = r;
	m_bits[idx + 1] = g;
	m_bits[idx + 2] = b;
	m_bits[idx + 3] = a;
}

void Texture2D::apply(bool readable)
{
	auto size = m_width * m_height * 4;

	const bgfx::Memory* mem;

	if (m_bitmap)
		mem = bgfx::makeRef(m_bits, size, releaseBits, static_cast<void*>(m_bitmap));
	else
		mem = bgfx::makeRef(m_bits, size, releaseBitsRaw, static_cast<void*>(m_bits));

	memcpy_s(mem->data, mem->size, m_bits, size);

	m_textureHandle = bgfx::createTexture2D(uint16_t(m_width), uint16_t(m_height), false, 1, bgfx::TextureFormat::RGBA8, m_flags, mem);
}

void Texture2D::dispose()
{
	bgfx::destroyTexture(m_textureHandle);
}

void Texture2D::loadTextureData(const char* filename, uint** pixels, int* width, int* height)
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
	*width = FreeImage_GetWidth(bitmap);
	*height = FreeImage_GetHeight(bitmap);

	*pixels = new uint[*width * *height];

	memcpy(*pixels, bits, *width * *height * 4);
}

void Texture2D::releaseTextureData(uint* pixels)
{
	delete pixels;
}

Ptr<Texture2D> Texture2D::createTexture()
{
	Ptr<Texture2D> texture(new Texture2D);
	return texture;
}
