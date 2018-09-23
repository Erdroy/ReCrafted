// ReCrafted (c) 2016-2018 Always Too Late

#include "AssetBase.h"

AssetBase::~AssetBase()
{
    if(!m_unloaded)
    {
        // Unload the asset when object is deleted
        Unload();
    }
}

void AssetBase::Deserialize(BinaryStream& stream)
{
    ASSERT(stream.IsOpen());

    ASSERT(stream.ReadByte() == 'R');
    ASSERT(stream.ReadByte() == 'C');
    ASSERT(stream.ReadByte() == 'A');

    cvar version = stream.ReadUInt16();
    cvar assetType = static_cast<AssetType>(stream.ReadByte());

    ASSERT(assetType == GetAssetType());

    OnDeserialize(version, stream);
}

void AssetBase::Unload()
{
    OnUnload();

    m_unloaded = true;
}
