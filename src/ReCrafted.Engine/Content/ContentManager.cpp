// ReCrafted (c) 2016-2018 Always Too Late

#include "ContentManager.h"
#include "Core/Streams/FileStream.h"
#include "Core/Logger.h"

#include <string>
#include <fstream>
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(ContentManager)

void ContentManager::OnInit()
{
}

void ContentManager::OnDispose()
{
    // Unload from queue
    UnloadAssets();

    // Clear deleted
    m_assetMap.clear_deleted_key();

    // Release all assets
    var numLeaked = 0;
    for(rvar assetPair : m_assetMap)
        numLeaked++;

    if(numLeaked > 0)
    {
        Logger::LogWarning("Detected leaked assets! {0} assets leaked!", numLeaked);
    }

    // Clear
    m_assetMap.clear();
}

void ContentManager::Update()
{
}

void ContentManager::OnFrameFinished()
{
    UnloadAssets();
}

void ContentManager::UnloadAssets()
{
    Profiler::BeginProfile(__FUNCTION__);

    // Unload all assets that are queued for unload
    Asset* asset;
    while (m_unloadQueue.try_dequeue(asset))
    {
        if (asset == nullptr)
            continue;

#ifdef _DEBUG
        Logger::Log("Unloading asset '{0}'", asset->AssetFile());
#endif

        ReleaseAsset(asset);
    }

    Profiler::EndProfile();
}

void ContentManager::RegisterAsset(Asset* asset)
{
    // Check main thread
    ASSERT(Platform::GetMainThreadId() == std::this_thread::get_id());

    // Set asset as registered
    asset->m_registered = true;

    m_assetMap[asset->GetAssetGuid()] = asset;
}

bool ContentManager::LoadAsset(Asset* asset, const char* name) const
{
    // Check if file exists
    if(!Platform::FileExists(name))
    {
        Logger::LogError("Failed to load asset. File '{1}' not found.", name);
        return true;
    }

#ifdef _DEBUG
    Logger::Log("Loading asset '{0}'", name);
#endif

    cvar assetBaseType = asset->GetAssetBaseType();
    switch (assetBaseType)
    {
    case AssetBaseType::Binary:
    {
        // Open streams
        var fileStream = FileStream(name, OpenMode::OpenRead);
        var binaryStream = BinaryStream(fileStream);

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
        cvar jsonData = nlohmann::json::parse(content);

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

    m_assetMap.erase(asset->GetAssetGuid());

    asset->OnUnload();

    // Delete asset
    Object::Destroy(asset);
}

Asset* ContentManager::LoadAssetSync(Asset* asset, const std::string& assetFile, const std::string& file)
{
    // Set asset name and file name
    asset->m_assetName = assetFile;
    asset->m_assetFile = file;

    asset->OnLoadBegin(file);

    if (m_instance->LoadAsset(asset, file.c_str()))
    {
        Object::Destroy(asset);
        return nullptr;
    }

    // Set asset as loaded and non-virtual
    asset->m_loaded = true;

    // Register and initialize asset
    m_instance->RegisterAsset(asset);
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
    var customTask = new AssetLoadTask();
    customTask->file = file;
    customTask->asset = asset;
    customTask->callback = onLoad;
    Task::CreateTask(customTask)->Queue();
}

void ContentManager::UnloadAsset(Asset* asset)
{
    ASSERT(m_instance);

    if(!asset->IsLoaded())
    {
        // Set asset unload flag
        // This flag should be used for every asset that implement own async loading.
        // Look: Texture::LoadTextureMipTask::Finish
        asset->m_unload = true;
        return;
    }

    // Queue for asset unloading
    m_instance->m_unloadQueue.enqueue(asset);
}
