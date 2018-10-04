// ReCrafted (c) 2016-2018 Always Too Late

#include "Texture.h"

void Texture::OnInitialize()
{
}

void Texture::OnLoadBegin(const std::string& fileName)
{
    // Load first mip

}

void Texture::OnDeserializeBinary(uint16_t version, BinaryStream& stream)
{
    m_format = stream.ReadUInt16();
    m_mips = stream.ReadByte();

    for(var mipId = 0u; mipId < m_mips; mipId ++)
    {
        cvar width = stream.ReadUInt16();
        cvar height = stream.ReadUInt16();
        cvar pitch = stream.ReadUInt16();
        cvar size = stream.ReadInt32();
        

    }
}

void Texture::OnUnload()
{
}

void Texture::LoadFromMemory(uint8_t* data, uint16_t width, uint16_t height, uint8_t mips, uint8_t bitsPerPixel)
{
}
