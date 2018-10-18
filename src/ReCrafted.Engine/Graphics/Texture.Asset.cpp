// ReCrafted (c) 2016-2018 Always Too Late

#include "Texture.h"
#include "Core/Logger.h"


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

    // Check if we must cancel loading, due to the texture is being unloaded
    if (texture->m_unload)
    {
        // Close file
        file.Close();

        // Finish loading
        texture->m_lazyLoading = false;

        // Unload the texture.
        texture->Unload();
        return;
    }

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

        // Finish loading
        texture->m_lazyLoading = false;

#ifdef _DEBUG
        Logger::Log("Texture asset '{0}' finished loading.", texture->AssetFile());
#endif
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
