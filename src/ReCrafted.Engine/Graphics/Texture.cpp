// ReCrafted (c) 2016-2018 Always Too Late

#include "Texture.h"

void Texture::OnInitialize()
{
    Apply();
}

void Texture::OnLoadBegin(const std::string& fileName)
{
    
}

void Texture::OnDeserializeBinary(uint16_t version, BinaryStream& stream)
{
    cvar totalSize = stream.ReadUInt32();
    m_format = stream.ReadUInt16();
    m_mips = stream.ReadByte();

    // TODO: Lazy texture loading

    // Allocate memory for this texture.
    cvar memory = reinterpret_cast<char*>(Renderer::Allocate(totalSize, 0));

    ASSERT(memory);

    var memoryOffset = 0;
    for(var mipId = 0u; mipId < m_mips; mipId ++)
    {
        cvar width = stream.ReadUInt16();
        cvar height = stream.ReadUInt16();
        cvar pitch = stream.ReadUInt16();
        cvar size = stream.ReadInt32();
        
        ASSERT(width > 0);
        ASSERT(height > 0);
        ASSERT(pitch > 0);
        ASSERT(size > 0);

        if(mipId == 0u)
        {
            m_width = width;
            m_height = height;
            m_pitch = pitch;
        }

        // Read mip data
        stream.ReadBytes(memory + memoryOffset, size);
        memoryOffset += size;
    }

    // Set memory pointer
    m_textureMemory = reinterpret_cast<Renderer::RendererMemory>(memory);
    m_textureMemorySize = totalSize;
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

void Texture::Initialize(const uint16_t width, const uint16_t height)
{
    cvar textureFormat = Renderer::TextureFormat::RGBA8;

    cvar pixelSize = Renderer::TextureFormatInfo[textureFormat][0];
    cvar pitch = width * pixelSize / 8;
    cvar textureSize = height * pitch;

    m_format = textureFormat;
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
    ASSERT(m_textureMemory);
    ASSERT(m_width > 0);
    ASSERT(m_height > 0);
    ASSERT(m_pitch > 0);

    // Convert dxgi format to texture format
    m_textureFormat = Renderer::DXGIFormatToTextureFormat(m_format);

    // Create texture
    m_textureHandle = Renderer::CreateTexture2D(m_width, m_height, m_pitch, m_mips, m_textureFormat, m_textureMemory, m_textureMemorySize, false, generateMips);
    Renderer::QueueFree(m_textureMemory);

    m_textureMemory = nullptr;
    m_applied = true;
}