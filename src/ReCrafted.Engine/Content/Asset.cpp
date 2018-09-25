// ReCrafted (c) 2016-2018 Always Too Late

#include "Asset.h"
#include "ContentManager.h"

Asset::~Asset()
{
    if(!m_unloaded)
    {
        // Unload the asset when object is deleted
        Unload();
    }
}

void Asset::Deserialize(BinaryStream& stream)
{
    ASSERT(stream.IsOpen());

    ASSERT(stream.ReadByte() == 'R');
    ASSERT(stream.ReadByte() == 'C');
    ASSERT(stream.ReadByte() == 'A');

    cvar version = stream.ReadUInt16();
    cvar assetType = static_cast<AssetType>(stream.ReadByte());
    stream.ReadBytes(reinterpret_cast<char*>(&m_AssetGuid.value), sizeof(Guid::_value));

    ASSERT(assetType == GetAssetType());

    OnDeserializeBinary(version, stream);
}

void Asset::Deserialize(const json& json, const std::string& content)
{
    // Read AssetVersion
    cvar assetVersion = json["AssetVersion"].get<uint16_t>();
    
    // Read AssetType
    cvar assetType = static_cast<AssetType>(json["AssetType"].get<byte>());
    cvar guidString = json["AssetGuid"].get<std::string>();
    SetAssetGuid(Guid::Parse(guidString));

    ASSERT(assetType == GetAssetType());

    OnDeserializeJson(assetVersion, json);
}

void Asset::Unload()
{
    if(!m_unloaded)
    {
        ContentManager::UnloadAsset(this);
        m_unloaded = true;
    }
}
