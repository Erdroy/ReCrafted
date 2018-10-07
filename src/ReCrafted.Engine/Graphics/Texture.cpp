// ReCrafted (c) 2016-2018 Always Too Late

#include "Texture.h"
#include "Core/Streams/FileStream.h"

#include <upng.h>

void Texture::LoadTextureMipTask::Execute(void* userData)
{
    var fs = FileStream(file);
    var stream = BinaryStream(fs);

    // Calculate mip offset
    cvar baseOffset = AssetHeaderSize + TextureHeaderSize;
    cvar mipOffset = baseOffset + texture->CalculateMipOffset(mipLevel);

    // Set mip offset
    stream.Position(mipOffset);

    // Read mip info
    cvar mipInfo = MipInfo::ReadMipInfo(stream);

    // Allocate data
    m_mipDataSize = mipInfo.size;
    m_mipData = Renderer::Allocate(m_mipDataSize, 0);

    // Read mip data
    stream.ReadBytes(static_cast<char*>(m_mipData), m_mipDataSize);
}

void Texture::LoadTextureMipTask::Finish()
{
    // Upload mip
    ASSERT(m_mipData);

    // Update texture mip and texture view, to set the loaded mip as current
    Renderer::UpdateTextureSubresource(texture->GetHandle(), m_mipData, m_mipDataSize, mipLevel);
    Renderer::UpdateTextureView(texture->GetHandle(), mipLevel, mipCount - mipLevel);
    Renderer::QueueFree(m_mipData);

    // Check if we have any mips left
    if (mipLevel > 0)
    {
        mipLevel--;

        // Queue texture mip update
        var task = new LoadTextureMipTask;
        task->texture = texture;
        task->mipLevel = mipLevel;
        task->mipCount = mipCount;
        task->file = file;

        var nextTask = Task::CreateTask(task);
        nextTask->Queue();
    }
    else
    {
        // Close file
        file.Close();
    }
}

Texture::TextureInfo Texture::TextureInfo::ReadTextureInfo(BinaryStream& stream)
{
    cvar size = static_cast<size_t>(stream.ReadUInt32());
    cvar format = stream.ReadUInt16();
    cvar mips = static_cast<uint8_t>(stream.ReadByte());

    ASSERT(size > 0);
    ASSERT(format > 0);
    ASSERT(mips > 0);

    TextureInfo result;
    result.size = size;
    result.format = format;
    result.mips = mips;
    return result;
}

Texture::MipInfo Texture::MipInfo::ReadMipInfo(BinaryStream& stream)
{
    cvar width = stream.ReadUInt16();
    cvar height = stream.ReadUInt16();
    cvar pitch = stream.ReadUInt16();
    cvar size = static_cast<size_t>(stream.ReadInt32());

    ASSERT(width > 0);
    ASSERT(height > 0);
    ASSERT(pitch > 0);
    ASSERT(size > 0);

    MipInfo result;
    result.width = width;
    result.height = height;
    result.pitch = pitch;
    result.size = size;
    return result;
}

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

void Texture::LoadTexture(BinaryStream& stream)
{
    // Read texture info
    cvar textureInfo = TextureInfo::ReadTextureInfo(stream);

    cvar totalSize = stream.ReadUInt32();
    m_format = textureInfo.format;
    m_mips = textureInfo.mips;

    ASSERT(stream.Position() == AssetHeaderSize + TextureHeaderSize);

    // Allocate memory for this texture.
    cvar memory = reinterpret_cast<char*>(Renderer::Allocate(totalSize, 0));

    ASSERT(memory);

    var memoryOffset = 0;
    for (var mipId = 0u; mipId < m_mips; mipId++)
    {
        // Read mip info
        cvar mipInfo = MipInfo::ReadMipInfo(stream);

        if (mipId == 0u)
        {
            m_width = mipInfo.width;
            m_height = mipInfo.height;
            m_pitch = mipInfo.pitch;
        }

        // Read mip data
        stream.ReadBytes(memory + memoryOffset, mipInfo.size);
        memoryOffset += mipInfo.size;
    }

    // Set memory pointer
    m_textureMemory = reinterpret_cast<Renderer::RendererMemory>(memory);
    m_textureMemorySize = totalSize;
}

void Texture::LazyLoadTexture(const TextureInfo& textureInfo, File& file, BinaryStream& stream)
{
    // Setup texture memory info
    m_textureMemory = nullptr;
    m_textureMemorySize = textureInfo.size;

    // Setup texture
    Apply();

    // Setup LoadTextureMipTask task
    LoadTextureMipTask task;
    task.mipLevel = m_mips - 1;
    task.mipCount = m_mips;
    task.texture = this;
    task.file = file;

    // Execute task
    task.Execute(nullptr);

    // Finalize task
    task.Finish(); // Finish queues next load task for higher mip.
}

void Texture::OnInitialize()
{
    // Apply texture when not loading as lazy texture.
    if (!m_lazyLoading)
    {
        Apply();
    }
}

void Texture::OnLoadBegin(const std::string& fileName)
{
    // Open file, fill will be closed when all mips will be loaded
    File file;
    Platform::OpenFile(&file, fileName.c_str(), OpenMode::OpenRead);
    file.AutoClose = false;

    // Setup texture (read initial data and setup memory)
    var fs = FileStream(file);
    var stream = BinaryStream(fs);

    // Skip header
    stream.Position(AssetHeaderSize);

    // Read texture info
    cvar textureInfo = TextureInfo::ReadTextureInfo(stream);
    m_format = textureInfo.format;
    m_mips = textureInfo.mips;

    ASSERT(stream.Position() == AssetHeaderSize + TextureHeaderSize);

    // Read highest mip metadata
    cvar mipInfo = MipInfo::ReadMipInfo(stream);
    ASSERT(stream.Position() == AssetHeaderSize + TextureHeaderSize + MipHeaderSize);
    m_width = mipInfo.width;
    m_height = mipInfo.height;
    m_pitch = mipInfo.pitch;

    if (m_mips == 1 && m_width > 16)
    {
        // Do not load as lazy, this could result as high CPU spike.
        m_lazyLoading = false;
        return;
    }

    // Set as lazy loading
    m_lazyLoading = true;

    // We can now proceed to load the texture
    LazyLoadTexture(textureInfo, file, stream);
}

void Texture::OnDeserializeBinary(uint16_t version, BinaryStream& stream)
{
    if (m_lazyLoading)
        return;

    // Load texture. This is not a lazy texture loading.
    LoadTexture(stream);
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