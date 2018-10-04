// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Core/Action.h"
#include "Core/TaskManager.h"

#include "Assets/Asset.h"

#include <spp.h>

class ContentManager : public EngineComponent<ContentManager>
{
private:
    friend class EngineMain;

private:
    struct AssetLoadTask : ITask
    {
    public:
        void Execute(void* userData) override
        {
            if(GetInstance()->LoadAsset(asset, file.c_str()))
            {
                // Unload asset and reset
                UnloadAsset(asset);
                asset = nullptr;
            }
        }

        void Finish() override
        {
            GetInstance()->RegisterAsset(asset);
            asset->OnInitialize();
            callback.Invoke(asset);
        }

    public:
        std::string file = nullptr;
        Asset* asset = nullptr;
        Action<void, Asset*> callback = nullptr;
    };

private:
    Array<Asset*> m_assets;
    spp::sparse_hash_map<Guid, Asset*> m_assetMap;
    moodycamel::ConcurrentQueue<Asset*> m_unloadQueue;

public:
    virtual ~ContentManager() = default;

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;
    void PreFrame();

private:
    void RegisterAsset(Asset* asset);
    bool LoadAsset(Asset* asset, const char* name) const;
    void ReleaseAsset(Asset* asset);

    /**
     * \brief Creates empty virtual asset of given type.
     * \tparam TAsset The target asset class type.
     * \return The created asset.
     */
    template<class TAsset>
    static TAsset* InternalCreateAsset()
    {
        cvar asset = new TAsset();
        return asset;
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
        cvar asset = InternalCreateAsset<TAsset>();
        asset->m_virtual = true;
        asset->SetAssetGuid(Platform::NewGuid());
        m_instance->RegisterAsset(asset);
        return asset;
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
        cvar it = m_instance->m_assetMap.find(guid);
        if(it != m_instance->m_assetMap.end())
            return static_cast<TAsset*>(it->second);

        return nullptr;
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
        cvar assetName = std::string(assetFile);
        cvar file = "../content/" + assetName + ".rcasset";

        cvar asset = InternalCreateAsset<TAsset>();
        asset->OnLoadBegin(file);
        if (m_instance->LoadAsset(asset, file.c_str()))
        {
            delete asset;
            return nullptr;
        }

        // Set asset as loaded and non-virtual
        asset->m_loaded = true;

        // Register and initialize asset
        m_instance->RegisterAsset(asset);
        asset->OnInitialize();
        return asset;
    }

    /**
     * \brief Loads asset of specified type from given file asynchronously.
     * \tparam TAsset The asset class type.
     * \param assetFile The asset file, relative to '../content/', file extension is not needed.
     * \param onLoad Load callback, this callback is dispatched after 
     * the asset has been loaded on the main thread, the first paramater is a pointer to the asset object.
     */
    template<class TAsset>
    static void LoadAsset(const char* assetFile, const Action<void, Asset*>& onLoad)
    {
        // Build file name
        cvar assetName = std::string(assetFile);
        cvar file = "../content/" + assetName + ".rcasset";

        cvar asset = InternalCreateAsset<TAsset>();
        asset->OnLoadBegin(file);

        // Create and queue task
        var customTask = new AssetLoadTask();
        customTask->file = file;
        customTask->asset = asset;
        customTask->callback = onLoad;
        Task::CreateTask(customTask)->Queue();
    }
};

#endif // CONTENTMANAGER_H
