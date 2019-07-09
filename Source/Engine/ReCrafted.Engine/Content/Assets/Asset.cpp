// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Asset.h"
#include "../ContentManager.h"

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

void Asset::UnloadNow()
{
    // Unload asset if not unloaded
    if (!m_unloaded)
        ContentManager::UnloadAsset(this, true);
}

void Asset::OnLoadEnd()
{
    // Set asset as loaded and non-virtual
    m_loaded = true;

    // Unload if needed
    if (m_unload)
        Destroy(this);
}

bool Asset::IsLoaded() const
{
    MAIN_THREAD_ONLY();
    return m_loaded || IsVirtual() || !IsLoading();
}

bool Asset::IsLoading() const
{
    MAIN_THREAD_ONLY();

    // Note: This can be called only from MainThread, and the AssetLoadTask::Finish is being called from Update loop, so
    // there is no way of getting IsLoading true, then asset get's loaded while we're adding our callback to load map.

    const auto contentManager = ContentManager::GetInstance();

    auto& lock = contentManager->m_assetLoadMapLick;
    ScopeLock(lock);

    // Try to find asset in the map
    const auto it = contentManager->m_assetLoadMap.find(const_cast<Asset*>(this));
    return it != contentManager->m_assetLoadMap.end();
}
