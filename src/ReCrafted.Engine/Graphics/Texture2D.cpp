// ReCrafted (c) 2016-2018 Always Too Late

#include "Texture2D.h"
#include "upng/upng.h"
#include "ReCrafted.h"
#include "Core/Delegate.h"
#include <functional>

void Texture2D::dispose()
{
    if (RENDERER_CHECK_HANDLE(m_textureHandle))
        Renderer::DestroyTexture2D(m_textureHandle);

    if (m_bits)
    {
        if (m_bitmap)
        {
            upng::upng_free(m_bitmap);
            m_bits = nullptr;
            m_bitmap = nullptr;
        }
        else
        {
            free(m_bits);
            m_bits = nullptr;
        }
    }
}

void Texture2D::onDestroy()
{
    dispose();
}

void Texture2D::releaseBitmap(void* ptr)
{
    upng::upng_free(static_cast<upng_t*>(ptr));
}

void Texture2D::releaseData(void* ptr)
{
    delete[] static_cast<byte*>(ptr);
}

void Texture2D::loadFile(const char* filename, uint flags)
{
	m_bitmap = upng::upng_new_from_file(filename);

	if (m_bitmap == nullptr)
		throw;

	upng::upng_decode(m_bitmap);

	m_bits = const_cast<byte*>(upng::upng_get_buffer(m_bitmap));
	m_width = upng::upng_get_width(m_bitmap);
	m_height = upng::upng_get_height(m_bitmap);
	m_bpp = upng::upng_get_bpp(m_bitmap);
	m_flags = flags;
	
	// done!

	// FreeImage bitmap will be unloaded with in `releaseBits`
}

void Texture2D::loadMemory(void* data, int width, int height, uint flags)
{
	m_width = width;
	m_height = height;
	m_flags = flags;

	m_bits = new byte[width * height * 4];

	if(data)
		memcpy(m_bits, data, width * height * 4);
}

void Texture2D::createMemory(int width, int height, uint flags)
{
	loadMemory(nullptr, width, height, flags);
}

uint Texture2D::getPixel(int x, int y)
{
	if (m_bits == nullptr)
		return 0x0u;

	cvar idx = y * m_width + x;

    cvar r = m_bits[idx + 0];
    cvar g = m_bits[idx + 1];
    cvar b = m_bits[idx + 2];
    cvar a = m_bits[idx + 3];

	return a | b << 8 | g << 16 | r << 24;
}

uint* Texture2D::getPixels() const
{
	return reinterpret_cast<uint*>(m_bits);
}

void Texture2D::setPixel(int x, int y, uint pixel)
{
	if (m_bits == nullptr)
		return;

    cvar idx = y * m_width + x;
    cvar pixels = reinterpret_cast<uint*>(m_bits);
	pixels[idx] = pixel;
}

void Texture2D::setPixel(int x, int y, byte r, byte g, byte b, byte a)
{
	if (m_bits == nullptr)
		return;

    cvar idx = y * m_width + x;
    cvar pixels = reinterpret_cast<uint*>(m_bits);
	pixels[idx] = a | b << 8 | g << 16 | r << 24;
}

void Texture2D::setPixels(int x, int y, int width, int height, uint* pixels)
{
    cvar mpixels = reinterpret_cast<uint*>(m_bits);
	for(auto my = y; my < y + height; my++)
	{
		for (auto mx = x; mx < x + width; mx++)
		{
			mpixels[my * m_width + mx] = *pixels++;
		}
	}
}

uint Texture2D::getWidth() const
{
	return m_width;
}

uint Texture2D::getHeight() const
{
	return m_height;
}

void Texture2D::apply()
{
    _ASSERT(m_bitmap || m_bits);

    cvar pitch = Renderer::TextureFormatInfo[m_textureFormat][0] / 8;
	var size = m_width * m_height * pitch;

	if (m_mips > 0)
	{
		var currentWidth = m_width;
		for (auto i = 0u; i < m_mips; i++)
		{
			cvar width = currentWidth / 2;
            currentWidth = width;

			size += width * width * 4;
		}
	}

    Renderer::RendererMemory memory;
    if (m_bitmap)
		memory = Renderer::Allocate(m_bitmap, std::function<void(void*)>(&Texture2D::releaseBitmap));
    else
        memory = Renderer::Allocate(m_bits, std::function<void(void*)>(&Texture2D::releaseData));

	cvar mipCount = uint8_t(m_mips + 1);
    m_textureHandle = Renderer::CreateTexture2D(uint16_t(m_width), uint16_t(m_height), mipCount, m_textureFormat, memory, size);
	m_bits = nullptr;
}

Ref<Texture2D> Texture2D::createTexture(Renderer::TextureFormat::_enum format)
{
	Ref<Texture2D> texture(new Texture2D);
    texture->m_textureFormat = format;
	return texture;
}