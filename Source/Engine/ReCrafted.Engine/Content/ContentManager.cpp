// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ContentManager.h"

#include <string>
#include <fstream>

void ContentManager::Initialize()
{
}

void ContentManager::Shutdown()
{
    m_assetMapLock.LockNow();
    // Clear deleted
    m_assetMapA.clear_deleted_key();

    // Release all assets
    List<Asset*> assets = {};
    for (auto&& assetPair : m_assetMapA)
    {
        const auto asset = assetPair.second;
        assets.Add(asset);
    }
    m_assetMapLock.UnlockNow();

    for(auto&& asset : assets)
    {
        // Unloading the asset without release is essential right there to omit deadlock later.
        UnloadAsset(asset, false);
        Object::DestroyNow(asset);
    }

    // Clear
    m_assetMapLock.LockNow();
    m_assetMapA.clear();
    m_assetMapLock.UnlockNow();
}

void ContentManager::OnUpdate()
{
}

void ContentManager::OnFrameDone()
{
}

void ContentManager::RegisterAsset(Asset* asset)
{
    // Check main thread
    MAIN_THREAD_ONLY();

    ScopeLock(m_assetMapLock);

    // Set asset as registered
    asset->m_registered = true;

    // Add asset to the map
    m_assetMapA.insert(std::make_pair(asset->AssetGuid, asset));

    // Add asset guid to the map
    m_assetMapB.insert(std::make_pair(std::string(asset->AssetName()), asset));
}

bool ContentManager::LoadAsset(Asset* asset, const char* name) const
{
    // Check if file exists
    if(!File::Exists(name))
    {
        Logger::LogError("Failed to load asset. File '{1}' not found.", name);
        return true;
    }

#ifdef _DEBUG
    Logger::Log("Loading asset '{0}'", name);
#endif

    const auto assetBaseType = asset->GetAssetBaseType();
    switch (assetBaseType)
    {
    case AssetBaseType::Binary:
    {
        // Open streams
        auto fileStream = FileStream(name, File::OpenMode::BinaryRead);
        auto binaryStream = BinaryStream(fileStream);

        ASSERT(fileStream.IsOpen());

        // Deserialize asset
        asset->Deserialize(binaryStream);

        // Dispose file stream
        fileStream.Dispose();
        break;
    }
    case AssetBaseType::Json:
    {
        // Load and parse Json data
        std::ifstream input(name);
        const std::string content((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
        const auto jsonData = nlohmann::json::parse(content);

        // Deserialize asset
        asset->Deserialize(jsonData, content);
        break;
    }
    default:
    {
        Logger::LogError("Invalid asset base type {0} for asset {1}", static_cast<int>(assetBaseType), name);
        return true;
    }
    }

    return false;
}

void ContentManager::ReleaseAsset(Asset* asset)
{
    ASSERT(asset);

    asset->m_loaded = false;
    asset->m_unloaded = true;

    m_assetMapLock.LockNow();
    m_assetMapA.erase(asset->AssetGuid);
    m_assetMapLock.UnlockNow();
}

void ContentManager::LoadAssetSync(Asset* asset, const std::string& assetFile, const std::string& file)
{
    // Set asset name and file name
    asset->m_assetName = assetFile;
    asset->m_assetFile = file;

    asset->OnLoadBegin(file);

    if (GetInstance()->LoadAsset(asset, file.c_str()))
    {
        Object::Destroy(asset);
        return;
    }

    // Register and initialize asset
    GetInstance()->RegisterAsset(asset);
    asset->OnInitialize();

    asset->OnLoadEnd();
}

void ContentManager::LoadAssetAsync(Asset* asset, const std::string& assetFile, const std::string& file,
    const Action<void, Asset*>& onLoad)
{
    const auto contentManager = GetInstance();

    // Set asset name and file name
    asset->m_assetName = assetFile;
    asset->m_assetFile = file;

    asset->OnLoadBegin(file);

    // Create and queue task
    auto customTask = new AssetLoadTask();
    customTask->file = file;
    customTask->asset = asset;
    customTask->onLoadedEvent.AddListener(onLoad);

    // Add asset to the load map
    contentManager->m_assetLoadMapLick.LockNow();
    {
        contentManager->m_assetLoadMap[asset] = customTask;
    }
    contentManager->m_assetLoadMapLick.UnlockNow();

    // Queue task for execution
    Task::CreateTask(customTask)->Queue();
}

void ContentManager::UnloadAsset(Asset* asset, const bool release)
{
    if (!asset->IsLoaded())
    {
        // Set asset unload flag
        // This flag should be used for every asset that implement own async loading.
        // Look: Texture::LoadTextureMipTask::Finish
        asset->m_unload = true;
        return;
    }

    // Reset ref count
    asset->ResetRefCount();

#ifdef _DEBUG
    Logger::Log("Unloading asset '{0}'", asset->AssetName());
#endif

    // Queue for asset unloading
    asset->m_unloaded = true;

    // Release
    if (release)
    {
        GetInstance()->ReleaseAsset(asset);
    }
}

void ContentManager::InternalInitVirtualAsset(Asset* asset)
{
    asset->m_virtual = true;
    asset->AssetGuid = Platform::NewGuid();
    GetInstance()->RegisterAsset(asset);
}

void ContentManager::InternalLoadAssetSync(Asset* asset, const char* assetFile)
{
    // Build file name
    const auto file = GetAssetFile(assetFile);

    // Load asset sync
    LoadAssetSync(asset, assetFile, file);
}

void ContentManager::InternalLoadAssetAsync(Asset* asset, const char* assetFile, const Action<void, Asset*>& onLoad)
{
    // Build file name
    const auto file = GetAssetFile(assetFile);

    // Load asset sync
    LoadAssetAsync(asset, assetFile, file, onLoad);
}

Asset* ContentManager::InternalFindAsset(const Guid& guid)
{
    auto& assetMapLock = GetInstance()->m_assetMapLock;
    ScopeLock(assetMapLock);

    const auto it = GetInstance()->m_assetMapA.find(guid);
    if (it != GetInstance()->m_assetMapA.end())
        return it->second;

    return nullptr;
}

Asset* ContentManager::InternalFindAsset(const char* assetFile)
{
    auto& assetMapLock = GetInstance()->m_assetMapLock;
    ScopeLock(assetMapLock);

    const auto it = GetInstance()->m_assetMapB.find(assetFile);
    if (it != GetInstance()->m_assetMapB.end())
        return it->second;

    return nullptr;
}

void ContentManager::InternalAddAssetLoadEvent(Asset* asset, const Action<void, Asset*>& onLoad)
{
#if DEBUG
    Logger::LogWarning("Asset {0} is loading.", asset->AssetFile());
#endif

    // Lock the asset load map
    const auto contentManager = ContentManager::GetInstance();
    auto& lock = contentManager->m_assetLoadMapLick;
    ScopeLock(lock);

    // Find asset's load task
    const auto it = contentManager->m_assetLoadMap.find(const_cast<Asset*>(asset));
    ASSERT(it != contentManager->m_assetLoadMap.end());
    auto task = dynamic_cast<AssetLoadTask*>(it->second);

    // Add load callback
    task->onLoadedEvent.AddListener(onLoad);
}
