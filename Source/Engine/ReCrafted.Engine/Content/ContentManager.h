// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/Logger.h"
#include "Common/Guid.h"
#include "Core/Threading/Task.h"
#include "Content/Assets/Asset.h"

class ContentManager final : public SubSystem<ContentManager>
{
private:
    friend class EngineMain;

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
    /**
     * \brief Creates empty virtual asset of given type.
     * \tparam TAsset The target asset class type.
     * \return The created asset.
     */
    template<class TAsset>
    static TAsset* InternalCreateAsset()
    {
        return Object::New<TAsset>();
    }

    static void InternalLoadAssetSync(Asset* asset, const char* assetFile)
    {
        // Build file name
        const auto file = GetAssetFile(assetFile);

        // Load asset sync
        LoadAssetSync(asset, assetFile, file);
    }

    static void InternalLoadAssetAsync(Asset* asset, const char* assetFile, const  Action<void, Asset*>& onLoad)
    {
        // Build file name
        const auto file = GetAssetFile(assetFile);

        // Load asset sync
        LoadAssetAsync(asset, assetFile, file, onLoad);
    }

    static void InternalInitVirtualAsset(Asset* asset)
    {
        asset->m_virtual = true;
        asset->AssetGuid = Platform::NewGuid();
        GetInstance()->RegisterAsset(asset);
    }

public:
    /**
     * \brief Creates empty virtual asset of given type.
     * \tparam TAsset The target asset class type.
     * \return The created asset.
     */
    template<class TAsset>
    static TAsset* CreateVirtualAsset()
    {
        const auto asset = static_cast<Asset*>(InternalCreateAsset<TAsset>());
        InternalInitVirtualAsset(asset);
        return static_cast<TAsset*>(asset);
    }

    /**
     * \brief Looks for asset with given guid.
     * \tparam TAsset The target asset class type.
     * \param guid The asset guid.
     * \return The found asset or null when not found.
     */
    template<class TAsset>
    static TAsset* FindAsset(const Guid& guid)
    {
        const auto it = GetInstance()->m_assetMap.find(guid);
        if(it != GetInstance()->m_assetMap.end())
            return static_cast<TAsset*>(it->second);

        return nullptr;
    }

    /**
     * \brief Safely unloads given asset and clears it's pointer.
     * \param asset The asset that will be unloaded.
     */
    template<class TAsset>
    FORCEINLINE static void UnloadAssetSafe(TAsset*& asset)
    {
        if (asset)
        {
            Object::Destroy(asset);
            asset = nullptr;
        }
    }

    /**
     * \brief Unloads given asset.
     * \param asset The asset that will be unloaded.
     */
    static void UnloadAsset(Asset* asset);

    /**
     * \brief Loads asset of specified type from given file.
     * \tparam TAsset The asset class type.
     * \param assetFile The asset file, relative to '../content/', file extension is not needed.
     * \return The created file or nullptr when failed.
     */
    template<class TAsset>
    static TAsset* LoadAsset(const char* assetFile)
    {
        // Build file name
        const auto file = GetAssetFile(assetFile);

        const auto asset = static_cast<Asset*>(InternalCreateAsset<TAsset>());
        return static_cast<TAsset*>(LoadAssetSync(asset, assetFile, file));
    }

    /**
     * \brief Loads asset of specified type from given file asynchronously.
     * \tparam TAsset The asset class type.
     * \param assetFile The asset file, relative to '../content/', file extension is not needed.
     * \param onLoad Load callback, this callback is dispatched after 
     * the asset has been loaded on the main thread, the first parameter is a pointer to the asset object.
     */
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
