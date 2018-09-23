// ReCrafted (c) 2016-2018 Always Too Late

#include "ContentManager.h"
#include "TestAsset.h"
#include "Core/Streams/FileStream.h"
#include "Core/Logger.h"

SINGLETON_IMPL(ContentManager)

void ContentManager::OnInit()
{
}

void ContentManager::OnDispose()
{
    // Release all assets
    for(rvar asset : m_assets)
        ReleaseAsset(asset);

    m_assets.Clear();
    m_assetMap.clear();
}

void ContentManager::Update()
{
}

void ContentManager::PreFrame()
{
    // TODO: Unload assets
}

void ContentManager::RegisterAsset(AssetBase* asset)
{
    // Check main thread
    ASSERT(Platform::GetMainThreadId() == std::this_thread::get_id());

    m_assetMap[asset->GetAssetGuid()] = asset;
    m_assets.Add(asset);
}

bool ContentManager::LoadAsset(AssetBase* asset, const char* name) const
{
    // Build file name
    cvar assetName = std::string(name);
    cvar assetFile = "../content/" + assetName + ".rcasset";

    // Check if file exists
    if(!Platform::FileExists(assetFile.c_str()))
    {
        Logger::LogError("Failed to load asset '{0}'. File '{1}' not found.", assetName.c_str(), assetFile.c_str());
        return true;
    }

#ifdef _DEBUG
    Logger::Log("Loading asset '{0}'", assetName.c_str());
#endif

    // Open streams
    var fileStream = FileStream(assetFile.c_str(), OpenMode::OpenRead);
    var binaryStream = BinaryStream(fileStream);

    ASSERT(fileStream.IsOpen());

    // Deserialize asset
    asset->Deserialize(binaryStream);
    
    // Dispose file stream
    fileStream.Dispose();
    return false;
}

void ContentManager::ReleaseAsset(AssetBase* asset)
{
    ASSERT(asset);

    m_assets.Remove(asset);
    m_assetMap.erase(asset->GetAssetGuid());

    asset->OnUnload();

    // Delete asset
    delete asset;
}

void ContentManager::UnloadAsset(AssetBase* asset)
{
    // Queue for asset unloading
    m_instance->m_unloadQueue.enqueue(asset);
}
