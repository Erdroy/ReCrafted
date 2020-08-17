// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Logger.h"
#include "Common/Event.h"
#include "Common/Guid.h"
#include "Common/IO/File.h"
#include "Content/Assets/Asset.h"
#include "Core/Threading/Task.h"

API_USING("System")

/// <summary>
///     ContentManager class. Provides asset management functionality (loading etc.).
/// </summary>
API_CLASS(public, static, partial, noinherit)
class ContentManager final : public SubSystem<ContentManager>
{
    API_CLASS_BODY()
    friend class Asset;

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
            // This function is being called on the main thread.

            if (!asset)
                return;

            const auto contentManager = GetInstance();

            contentManager->RegisterAsset(asset);

            asset->OnLoadEnd();
            
            // Set asset as loaded and non-virtual
            asset->m_loaded = true;
            asset->OnInitialize();

            // Remove asset from the load map
            contentManager->m_assetLoadMapLick.LockNow();

            const auto it = contentManager->m_assetLoadMap.find(asset);
            if (it != contentManager->m_assetLoadMap.end())
                contentManager->m_assetLoadMap.erase(it);

            // Invoke loaded event
            onLoadedEvent.Invoke(asset);

            contentManager->m_assetLoadMapLick.UnlockNow();
        }

    public:
        std::string file;
        Asset* asset = nullptr;
        Event<Asset*> onLoadedEvent {};
    };

private:
    spp::sparse_hash_map<Guid, Asset*> m_assetMapA;
    spp::sparse_hash_map<std::string, Asset*> m_assetMapB;
    Lock m_assetMapLock;

    spp::sparse_hash_map<Asset*, ITask*> m_assetLoadMap;
    Lock m_assetLoadMapLick;

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

    static void LoadAssetSync(Asset* asset, const std::string& assetFile, const std::string& file);
    static void LoadAssetAsync(Asset* asset, const std::string& assetFile, const std::string& file, const Action<void, Asset*>& onLoad);
    static void UnloadAsset(Asset* asset, bool release = true);

public:
    API_FUNCTION(noproxy)
    static void InternalInitVirtualAsset(Asset* asset);

    API_FUNCTION(noproxy)
    static void InternalLoadAssetSync(Asset* asset, const char* assetFile);

    API_FUNCTION(noproxy)
    static void InternalLoadAssetAsync(Asset* asset, const char* assetFile, const Action<void, Asset*>& onLoad);

    API_FUNCTION(noproxy)
    static void InternalAddAssetLoadEvent(Asset* asset, const Action<void, Asset*>& onLoad);

    API_FUNCTION(noproxy)
    static Asset* InternalFindAsset(const Guid& guid);

    API_FUNCTION(noproxy)
    static Asset* InternalFindAsset(const char* assetFile);

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
        MAIN_THREAD_ONLY();
        return Object::New<TAsset>();
    }

public:
    /// <summary>
    ///     Creates empty virtual asset of given type.
    /// </summary>
    /// <typeparam name="TAsset">The target asset class type.</typeparam>
    /// <returns>The created asset.</returns>
    /// <remarks>Asset object must have it's API implemented!</remarks>
    /// <remarks>Can be called only from the MainThread.</remarks>
    template<class TAsset>
    static TAsset* CreateVirtualAsset()
    {
        MAIN_THREAD_ONLY();
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
        return static_cast<TAsset*>(InternalFindAsset(guid));
    }

    /// <summary>
    ///     Looks for asset with given asset file name.
    /// </summary>
    /// <typeparam name="TAsset">The target asset class type.</typeparam>
    /// <param name="assetFile">The asset file, relative to '../Content/', file extension is not needed.</param>
    /// <returns>The found asset or null when not found.</returns>
    template<class TAsset>
    static TAsset* FindAsset(const char* assetFile)
    {
        return static_cast<TAsset*>(InternalFindAsset(assetFile));
    }

    /// <summary>
    ///     Safely unloads given asset and clears it's pointer.
    /// </summary>
    /// <typeparam name="TAsset">The target asset class type.</typeparam>
    /// <param name="asset">The asset that will be unloaded.</param>
    /// <remarks>Can be called only from the MainThread.</remarks>
    template<class TAsset>
    FORCEINLINE static void UnloadAssetSafe(TAsset*& asset)
    {
        MAIN_THREAD_ONLY();
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
    /// <remarks>Can be called only from the MainThread.</remarks>
    template<class TAsset>
    static TAsset* LoadAsset(const char* assetFile)
    {
        MAIN_THREAD_ONLY();
        static_assert(std::is_base_of<Asset, TAsset>::value, "TAsset must inherit Asset class!");

        // Build file name
        const auto file = GetAssetFile(assetFile);
        ASSERT(File::Exists(file.c_str()));

        // Try to find asset
        auto asset = static_cast<Asset*>(FindAsset<TAsset>(assetFile));

        if (asset)
        {
            // Asset already loaded, so just add reference.
            asset->AddRef();
            return static_cast<TAsset*>(asset);
        }
        
        asset = static_cast<Asset*>(InternalCreateAsset<TAsset>());
        LoadAssetSync(asset, assetFile, file);
        return static_cast<TAsset*>(asset);
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
    /// <remarks>Can be called only from the MainThread.</remarks>
    template<class TAsset>
    static TAsset* LoadAsset(const char* assetFile, const Action<void, Asset*>& onLoad)
    {
        MAIN_THREAD_ONLY();
        static_assert(std::is_base_of<Asset, TAsset>::value, "TAsset must inherit Asset class!");

        // Build file name
        const auto file = GetAssetFile(assetFile);
        ASSERT(File::Exists(file.c_str()));

        // Try to find asset
        auto asset = static_cast<Asset*>(FindAsset<TAsset>(assetFile));

        if (asset)
        {
            // Asset already loaded, so just add reference.
            asset->AddRef();

            if(asset->IsLoading())
            {
                InternalAddAssetLoadEvent(asset, onLoad);
            }
            else
            {
                // Invoke load callback
                const_cast<Action<void, Asset*>&>(onLoad).Invoke(asset);
            }

            return static_cast<TAsset*>(asset);
        }
        
        asset = static_cast<Asset*>(InternalCreateAsset<TAsset>());
        if (!asset->CanLoadAsync())
        {
            // Write warning
            Logger::LogWarning("Asset {0} should not be loaded async.", assetFile);

            // Load asset synchronously
            LoadAssetSync(asset, assetFile, file);
            const_cast<Action<void, Asset*>&>(onLoad).Invoke(asset);
            return static_cast<TAsset*>(asset);
        }

        LoadAssetAsync(asset, assetFile, file, onLoad);
        return static_cast<TAsset*>(asset);
    }
};
