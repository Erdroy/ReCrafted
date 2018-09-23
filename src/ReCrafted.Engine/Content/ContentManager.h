// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "AssetBase.h"

class ContentManager : public EngineComponent<ContentManager>
{
private:
    friend class EngineMain;

private:

public:
    virtual ~ContentManager() = default;

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;

private:
    void RegisterAsset(AssetBase* asset);
    bool LoadAsset(AssetBase* asset, const char* name) const;

public:
    /**
     * \brief Creates empty (virtual) asset of given type.
     * \tparam TAsset The asset class type.
     * \return The created asset.
     */
    template<class TAsset>
    TAsset* CreateAsset()
    {
        cvar asset = new TAsset();
        RegisterAsset(asset);
        return asset;
    }

public:
    /**
     * \brief Loads asset of specified type from given file.
     * \tparam TAsset The asset class type.
     * \param assetFile The asset file, relative to '../content/', file extension is not needed.
     * \return The created file or nullptr when failed.
     */
    template<class TAsset>
    TAsset* LoadAsset(const char* assetFile)
    {
        cvar asset = CreateAsset<TAsset>();
        if (LoadAsset(asset, assetFile))
            return nullptr;
        return asset;
    }
};

#endif // CONTENTMANAGER_H
