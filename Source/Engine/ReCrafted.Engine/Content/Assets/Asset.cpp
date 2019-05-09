// ReCrafted (c) 2016-2019 Always Too Late

#include "Asset.h"
#include "../ContentManager.h"

Asset::~Asset()
{
    if (!m_unloaded)
    {
        ContentManager::UnloadAsset(this);
    }
}

void Asset::Deserialize(BinaryStream& stream)
{
    ASSERT(stream.IsOpen());

    ASSERT(stream.ReadByte() == 'R');
    ASSERT(stream.ReadByte() == 'C');
    ASSERT(stream.ReadByte() == 'A');

    const auto version = stream.ReadUInt16();
    const auto assetType = static_cast<AssetType>(stream.ReadByte());
    stream.ReadBytes(reinterpret_cast<char*>(&AssetGuid.value), sizeof(Guid::_value));

    ASSERT(assetType == GetAssetType());

    // Check read position
    ASSERT(stream.Position() == AssetHeaderSize);

    OnDeserializeBinary(version, stream);
}

void Asset::Deserialize(const json& json, const std::string& content)
{
    // Read AssetVersion
    const auto assetVersion = json["AssetVersion"].get<uint16_t>();
    
    // Read AssetType
    const auto assetType = static_cast<AssetType>(json["AssetType"].get<byte>());
    const auto guidString = json["AssetGuid"].get<std::string>();
    AssetGuid = Guid::Parse(guidString);

    ASSERT(assetType == GetAssetType());

    OnDeserializeJson(assetVersion, json);
}

void Asset::OnLoadEnd()
{
    // Unload if needed
    if (m_unload)
        Destroy(this);
}
