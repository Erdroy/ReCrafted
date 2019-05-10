// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/Logger.h"
#include "Common/Guid.h"
#include "Core/Threading/Task.h"
#include "Content/Assets/Asset.h"

/// <summary>
///     ContentManager class. Provides asset management functionality (loading etc.).
/// </summary>
API_CLASS(public, static, partial, noinherit)
class ContentManager final : public SubSystem<ContentManager>
{
    API_CLASS_BODY()

private:
    struct AssetLoadTask final : ITask
    {
    public:
        void Execute(void* userData) override
        {
            if(GetInstance()->LoadAsset(asset, file.c_str()))
            {
                // Unload asset and reset
                Object::Destroy(asset);
                asset = nullptr;
            }
        }

        void Finish() override
        {
            if (!asset)
                return;

            GetInstance()->RegisterAsset(asset);

            asset->OnLoadEnd();

            // Set asset as loaded and non-virtual
            asset->m_loaded = true;
            asset->OnInitialize();
            callback.Invoke(asset);
        }

    public:
        std::string file;
        Asset* asset = nullptr;
        Action<void, Asset*> callback = nullptr;
    };

private:
    spp::sparse_hash_map<Guid, Asset*> m_assetMap;
    Lock m_assetMapLock;

private:
    void Initialize() override;
    void Shutdown() override;
    void OnUpdate() override;
    void OnFrameDone() override;

private:
    void RegisterAsset(Asset* asset);
    bool LoadAsset(Asset* asset, const char* name) const;
    void ReleaseAsset(Asset* asset);

private:
    static std::string GetAssetFile(const char* assetFile)
    {
        const auto assetName = std::string(assetFile);
        return "../Content/" + assetName + ".rcasset";
    }

    static Asset* LoadAssetSync(Asset* asset, const std::string& assetFile, const std::string& file);
    static void LoadAssetAsync(Asset* asset, const std::string& assetFile, const std::string& file, const Action<void, Asset*>& onLoad);

public:
    /// <summary>
    ///     Creates empty virtual asset of given type.
    /// </summary>
    /// <typeparam name="TAsset">The target asset class type.</typeparam>
    /// <returns>The created asset.</returns>
    /// <remarks>Asset object must have it's API implemented!</remarks>
    template<class TAsset>
    static TAsset* InternalCreateAsset()
    {
        return Object::New<TAsset>();
    }

    API_FUNCTION(noproxy)
    static void InternalInitVirtualAsset(Asset* asset)
    {
        asset->m_virtual = true;
        asset->AssetGuid = Platform::NewGuid();
        GetInstance()->RegisterAsset(asset);
    }

    API_FUNCTION(noproxy)
    static void InternalLoadAssetSync(Asset* asset, const char* assetFile)
    {
        // Build file name
        const auto file = GetAssetFile(assetFile);

        // Load asset sync
        LoadAssetSync(asset, assetFile, file);
    }

    static void InternalLoadAssetAsync(Asset* asset, const char* assetFile, const Action<void, Asset*>& onLoad)
    {
        // Build file name
        const auto file = GetAssetFile(assetFile);

        // Load asset sync
        LoadAssetAsync(asset, assetFile, file, onLoad);
    }

    static void UnloadAsset(Asset* asset);

public:
    /// <summary>
    ///     Creates empty virtual asset of given type.
    /// </summary>
    /// <typeparam name="TAsset">The target asset class type.</typeparam>
    /// <returns>The created asset.</returns>
    /// <remarks>Asset object must have it's API implemented!</remarks>
    template<class TAsset>
    static TAsset* CreateVirtualAsset()
    {
        const auto asset = static_cast<Asset*>(InternalCreateAsset<TAsset>());
        InternalInitVirtualAsset(asset);
        return static_cast<TAsset*>(asset);
    }

    /// <summary>
    ///     Looks for asset with given guid.
    /// </summary>
    /// <typeparam name="TAsset">The target asset class type.</typeparam>
    /// <param name="guid">The asset guid.</param>
    /// <returns>The found asset or null when not found.</returns>
    template<class TAsset>
    static TAsset* FindAsset(const Guid& guid)
    {
        const auto it = GetInstance()->m_assetMap.find(guid);
        if(it != GetInstance()->m_assetMap.end())
            return static_cast<TAsset*>(it->second);

        return nullptr;
    }

    /// <summary>
    ///     Safely unloads given asset and clears it's pointer.
    /// </summary>
    /// <typeparam name="TAsset">The target asset class type.</typeparam>
    /// <param name="asset">The asset that will be unloaded.</param>
    template<class TAsset>
    FORCEINLINE static void UnloadAssetSafe(TAsset*& asset)
    {
        if (asset)
        {
            Object::Destroy(asset);
            asset = nullptr;
        }
    }

    /// <summary>
    ///     Loads given asset file with given target asset type.
    /// </summary>
    /// <typeparam name="TAsset">The asset type.</typeparam>
    /// <param name="assetFile">The asset file, relative to '../Content/', file extension is not needed.</param>
    /// <returns>The created file or nullptr when failed.</returns>
    template<class TAsset>
    static TAsset* LoadAsset(const char* assetFile)
    {
        // Build file name
        const auto file = GetAssetFile(assetFile);

        const auto asset = static_cast<Asset*>(InternalCreateAsset<TAsset>());
        return static_cast<TAsset*>(LoadAssetSync(asset, assetFile, file));
    }

    /// <summary>
    ///     Loads given asset file with given target asset type.
    /// </summary>
    /// <typeparam name="TAsset">The asset type.</typeparam>
    /// <param name="assetFile">The asset file, relative to '../Content/', file extension is not needed.</param>
    /// <param name="onLoad">
    ///     Load callback, this callback is dispatched after 
    ///     the asset has been loaded on the main thread, the first parameter is a pointer to the asset object.
    /// </param>
    /// <returns>The created file or nullptr when failed.</returns>
    template<class TAsset>
    static void LoadAsset(const char* assetFile, const Action<void, Asset*>& onLoad)
    {
        // Build file name
        const auto file = GetAssetFile(assetFile);
        const auto asset = static_cast<Asset*>(InternalCreateAsset<TAsset>());

        if(asset->CanLoadAsync())
        {
            LoadAssetAsync(asset, assetFile, file, onLoad);
        }
        else
        {
            // Write warning
            Logger::LogWarning("Asset {0} should not be loaded as async.", assetFile);

            // Load asset synchronously
            LoadAssetSync(asset, assetFile, file);
            const_cast<Action<void, Asset*>&>(onLoad).Invoke(asset);
        }
    }
};
