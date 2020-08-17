// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Texture.h"

extern "C" {
    #include <upng.h>
}
size_t Texture::CalculateMipSize(const uint8_t mipId) const
{
    const auto width = m_width >> mipId;
    const auto height = m_height >> mipId;
    return Renderer::CalculateTextureSize(width, height, m_textureFormat);
}

size_t Texture::CalculateMipOffset(const uint8_t mipId) const
{
    // First mip has no offset
    if (mipId == 0)
        return 0;

    // Calculate first mip size
    size_t offset = 0u;
    for (auto i = 0; i < mipId; i++)
    {
        const auto mipSize = CalculateMipSize(i);
        offset += mipSize;
        offset += MipHeaderSize;
    }

    return offset;
}

void Texture::OnUnload()
{
    if (m_textureMemory)
    {
        Renderer::Free(m_textureMemory);
    }

    if (RENDERER_CHECK_HANDLE(m_textureHandle))
    {
        Renderer::DestroyTexture2D(m_textureHandle);
    }
}

void Texture::Initialize(const uint16_t width, const uint16_t height, Renderer::TextureFormat::_enum format)
{
    if (format == Renderer::TextureFormat::Unknown)
        format = Renderer::TextureFormat::RGBA8;

    const auto pixelSize = Renderer::TextureFormatInfo[format][0];
    const auto pitch = width * pixelSize / 8;
    const auto textureSize = height * pitch;

    m_format = format;
    m_width = width;
    m_height = height;
    m_pitch = pitch;

    m_textureMemory = Renderer::Allocate(textureSize, 0);
    m_textureMemorySize = textureSize;
}

void Texture::InitializeFromMemory(uint8_t* data, const size_t dataSize, const uint16_t width, const uint16_t height, const uint8_t mipNum,
    const Renderer::TextureFormat::_enum format)
{
    const auto pixelSize = Renderer::TextureFormatInfo[format][0];
    const auto pitch = width * pixelSize / 8;

    m_format = format;
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
    const auto bitmap = upng_new_from_file(fileName);

    ASSERT(bitmap);
    ASSERT(upng_decode(bitmap) == UPNG_EOK);

    const auto bits = const_cast<byte*>(upng_get_buffer(bitmap));
    const auto width = upng_get_width(bitmap);
    const auto height = upng_get_height(bitmap);
    const auto bpp = upng_get_bpp(bitmap);
    const auto size = upng_get_size(bitmap);

    _ASSERT_(bpp == 32, "Unsupported texture format! Texture must have 32bpp!");

    InitializeFromMemory(bits, size, width, height, 1, Renderer::TextureFormat::RGBA8);

    upng_free(bitmap);
}


uint Texture::GetPixel(const int x, const int y) const
{
    ASSERT(m_applied);
    _ASSERT_(m_textureMemory, "Cannot get pixel from this texture. Texture not initialized.");
    _ASSERT_(m_textureFormat == Renderer::TextureFormat::RGBA8, "Cannot get pixel from this texture. Invalid texture format. Must be RGBA8.");

    const auto bits = static_cast<uint8_t*>(m_textureMemory);
    const auto idx = y * m_width + x;

    const auto r = bits[idx + 0];
    const auto g = bits[idx + 1];
    const auto b = bits[idx + 2];
    const auto a = bits[idx + 3];

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

    const auto bits = static_cast<uint8_t*>(m_textureMemory);

    const auto idx = y * m_width + x;
    const auto pixels = reinterpret_cast<uint*>(bits);
    pixels[idx] = pixel;
}

void Texture::SetPixel(const int x, const int y, const byte r, const byte g, const byte b, const byte a) const
{
    ASSERT(m_applied);
    _ASSERT_(m_textureMemory, "Cannot get pixel from this texture. Texture not initialized.");
    _ASSERT_(m_textureFormat == Renderer::TextureFormat::RGBA8, "Cannot get pixel from this texture. Invalid texture format. Must be RGBA8.");

    const auto bits = static_cast<uint8_t*>(m_textureMemory);

    const auto idx = y * m_width + x;
    const auto pixels = reinterpret_cast<uint*>(bits);
    pixels[idx] = a | b << 8 | g << 16 | r << 24;
}

void Texture::SetPixels(const int x, const int y, const int width, const int height, uint * pixels) const
{
    ASSERT(m_applied);
    _ASSERT_(m_textureMemory, "Cannot get pixel from this texture. Texture not initialized.");

    const auto bits = static_cast<uint8_t*>(m_textureMemory);
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
    if (m_textureMemory)
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
