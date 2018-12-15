// ReCrafted (c) 2016-2018 Always Too Late

#include "Texture.h"

#include <upng.h>

size_t Texture::CalculateMipSize(const uint8_t mipId) const
{
    cvar width = m_width >> mipId;
    cvar height = m_height >> mipId;
    return Renderer::CalculateTextureSize(width, height, m_textureFormat);
}

size_t Texture::CalculateMipOffset(const uint8_t mipId) const
{
    // First mip has no offset
    if (mipId == 0)
        return 0;

    // Calculate first mip size
    size_t offset = 0u;
    for(var i = 0; i < mipId; i ++)
    {
        cvar mipSize = CalculateMipSize(i);
        offset += mipSize;
        offset += MipHeaderSize;
    }

    return offset;
}

void Texture::OnUnload()
{
    if(m_textureMemory)
    {
        Renderer::Free(m_textureMemory);
    }

    if (RENDERER_CHECK_HANDLE(m_textureHandle))
    {
        Renderer::DestroyTexture2D(m_textureHandle);
    }
}

void Texture::Initialize(const uint16_t width, const uint16_t height, const Renderer::TextureFormat::_enum format)
{
    cvar pixelSize = Renderer::TextureFormatInfo[format][0];
    cvar pitch = width * pixelSize / 8;
    cvar textureSize = height * pitch;

    m_format = format;
    m_width = width;
    m_height = height;
    m_pitch = pitch;

    m_textureMemory = Renderer::Allocate(textureSize, 0);
    m_textureMemorySize = textureSize;
}

void Texture::InitializeFromMemory(uint8_t* data, const size_t dataSize, const uint16_t width, const uint16_t height, const uint8_t mipNum, 
    const Renderer::TextureFormat::_enum textureFormat)
{
    cvar pixelSize = Renderer::TextureFormatInfo[textureFormat][0];
    cvar pitch = width * pixelSize / 8;

    m_format = textureFormat;
    m_width = width;
    m_height = height;
    m_pitch = pitch;
    m_mips = mipNum;

    m_textureMemory = Renderer::Allocate(dataSize, 0);
    m_textureMemorySize = dataSize;

    memcpy(m_textureMemory, data, dataSize);
}

void Texture::InitializeFromFile(const char* fileName)
{
    cvar bitmap = upng::upng_new_from_file(fileName);

    ASSERT(bitmap);
    ASSERT(upng::upng_decode(bitmap) == UPNG_EOK);

    cvar bits = const_cast<byte*>(upng::upng_get_buffer(bitmap));
    cvar width = upng::upng_get_width(bitmap);
    cvar height = upng::upng_get_height(bitmap);
    cvar bpp = upng::upng_get_bpp(bitmap);
    cvar size = upng::upng_get_size(bitmap);

    _ASSERT_(bpp == 32, "Unsupported texture format! Texture must have 32bpp!");

    InitializeFromMemory(bits, size, width, height, 1, Renderer::TextureFormat::RGBA8);

    upng::upng_free(bitmap);
}


uint Texture::GetPixel(const int x, const int y) const
{
    ASSERT(m_applied);
    _ASSERT_(m_textureMemory, "Cannot get pixel from this texture. Texture not initialized.");
    _ASSERT_(m_textureFormat == Renderer::TextureFormat::RGBA8, "Cannot get pixel from this texture. Invalid texture format. Must be RGBA8.");

    cvar bits = static_cast<uint8_t*>(m_textureMemory);
    cvar idx = y * m_width + x;

    cvar r = bits[idx + 0];
    cvar g = bits[idx + 1];
    cvar b = bits[idx + 2];
    cvar a = bits[idx + 3];

    return a | b << 8 | g << 16 | r << 24;
}

uint* Texture::GetPixels() const
{
    ASSERT(m_applied);
    _ASSERT_(m_textureMemory, "Cannot get pixel from this texture. Texture not initialized.");

    return static_cast<uint*>(m_textureMemory);
}

void Texture::SetPixel(const int x, const int y, const uint pixel) const
{
    ASSERT(m_applied);
    _ASSERT_(m_textureMemory, "Cannot get pixel from this texture. Texture not initialized.");
    _ASSERT_(m_textureFormat == Renderer::TextureFormat::RGBA8, "Cannot get pixel from this texture. Invalid texture format. Must be RGBA8.");
    
    cvar bits = static_cast<uint8_t*>(m_textureMemory);

    cvar idx = y * m_width + x;
    cvar pixels = reinterpret_cast<uint*>(bits);
    pixels[idx] = pixel;
}

void Texture::SetPixel(const int x, const int y, const byte r, const byte g, const byte b, const byte a) const
{
    ASSERT(m_applied);
    _ASSERT_(m_textureMemory, "Cannot get pixel from this texture. Texture not initialized.");
    _ASSERT_(m_textureFormat == Renderer::TextureFormat::RGBA8, "Cannot get pixel from this texture. Invalid texture format. Must be RGBA8.");

    cvar bits = static_cast<uint8_t*>(m_textureMemory);

    cvar idx = y * m_width + x;
    cvar pixels = reinterpret_cast<uint*>(bits);
    pixels[idx] = a | b << 8 | g << 16 | r << 24;
}

void Texture::SetPixels(const int x, const int y, const int width, const int height, uint* pixels) const
{
    ASSERT(m_applied);
    _ASSERT_(m_textureMemory, "Cannot get pixel from this texture. Texture not initialized.");

    cvar bits = static_cast<uint8_t*>(m_textureMemory);
    for (auto my = y; my < y + height; my++)
    {
        for (auto mx = x; mx < x + width; mx++)
        {
            bits[my * m_width + mx] = *pixels++;
        }
    }
}

void Texture::Apply(const bool generateMips)
{
    ASSERT(m_applied == false);
    ASSERT(m_width > 0);
    ASSERT(m_height > 0);
    ASSERT(m_pitch > 0);

    // Convert dxgi format to texture format
    m_textureFormat = Renderer::DXGIFormatToTextureFormat(m_format);

    // Create texture
    m_textureHandle = Renderer::CreateTexture2D(m_width, m_height, m_pitch, m_mips, m_textureFormat, m_textureMemory, m_textureMemorySize, false, generateMips);
    
    // Queue texture memory to free, when there is texture mmeory allocated.
    if(m_textureMemory)
    {
        Renderer::QueueFree(m_textureMemory);
    }

    m_textureMemory = nullptr;
    m_applied = true;
}

void Texture::Resize(const uint16_t width, const uint16_t height)
{
    ASSERT(width > 0);
    ASSERT(height > 0);

    m_width = width;
    m_height = height;

    // Release current texture
    OnUnload();

    // Apply the texture once again
    m_applied = false;
    Apply();
}
