// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ContentManager.h"

#include <string>
#include <fstream>

void ContentManager::Initialize()
{
}

void ContentManager::Shutdown()
{
    ScopeLock(m_assetMapLock);

    // Clear deleted
    m_assetMap.clear_deleted_key();

    // Release all assets
    auto numLeaked = 0;
    for (auto&& assetPair : m_assetMap)
        numLeaked++;

    if (numLeaked > 0)
    {
        Logger::LogWarning("Detected leaked assets! {0} assets leaked!", numLeaked);
    }

    // Clear
    m_assetMap.clear();
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

    // Set asset as registered
    asset->m_registered = true;

    ScopeLock(m_assetMapLock);
    m_assetMap[asset->AssetGuid] = asset;
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
    m_assetMap.erase(asset->AssetGuid);
    m_assetMapLock.UnlockNow();

    asset->OnUnload();
}

Asset* ContentManager::LoadAssetSync(Asset* asset, const std::string& assetFile, const std::string& file)
{
    // Set asset name and file name
    asset->m_assetName = assetFile;
    asset->m_assetFile = file;

    asset->OnLoadBegin(file);

    if (GetInstance()->LoadAsset(asset, file.c_str()))
    {
        Object::Destroy(asset);
        return nullptr;
    }

    // Set asset as loaded and non-virtual
    asset->m_loaded = true;

    // Register and initialize asset
    GetInstance()->RegisterAsset(asset);
    asset->OnInitialize();

    asset->OnLoadEnd();

    return asset;
}

void ContentManager::LoadAssetAsync(Asset* asset, const std::string& assetFile, const std::string& file,
    const Action<void, Asset*>& onLoad)
{
    // Set asset name and file name
    asset->m_assetName = assetFile;
    asset->m_assetFile = file;

    asset->OnLoadBegin(file);

    // Create and queue task
    auto customTask = new AssetLoadTask();
    customTask->file = file;
    customTask->asset = asset;
    customTask->callback = onLoad;
    Task::CreateTask(customTask)->Queue();
}

void ContentManager::UnloadAsset(Asset* asset)
{
    if(!asset->IsLoaded())
    {
        // Set asset unload flag
        // This flag should be used for every asset that implement own async loading.
        // Look: Texture::LoadTextureMipTask::Finish
        asset->m_unload = true;
        return;
    }

#ifdef _DEBUG
    Logger::Log("Unloading asset '{0}'", asset->AssetFile());
#endif

    // Queue for asset unloading
    asset->m_unloaded = true;

    // Release
    GetInstance()->ReleaseAsset(asset);
}
