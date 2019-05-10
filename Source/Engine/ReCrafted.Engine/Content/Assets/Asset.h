// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/IO/BinaryStream.h"
#include "Common/Guid.h"
#include "Content/Assets/AssetBaseType.h"
#include "Content/Assets/AssetType.h"
#include "Scripting/Object.h"

#include <nlohmann/json.hpp>
using namespace nlohmann;

/// <summary>
///     Asset class. Base class for all assets.
/// </summary>
API_CLASS(public, abstract, partial, customNamespace="ReCrafted.API.Content")
class Asset : public Object
{
    friend class ContentManager;
    API_CLASS_BODY()

public:
    static const size_t AssetHeaderSize = 
        3 * sizeof(uint8_t) +   // RCA
        sizeof(uint16_t) +      // Version
        sizeof(uint8_t) +       // Type
        sizeof(Guid::_value);   // Guid

protected:
    bool m_virtual = false;
    bool m_registered = false;
    bool m_loaded = false;
    bool m_unloaded = false;
    bool m_unload = false;

    std::string m_assetName = {};
    std::string m_assetFile = {};

public:
    Asset() = default;
    ~Asset();

protected:
    void Deserialize(BinaryStream& stream);
    void Deserialize(const json& json, const std::string& content);

protected:
    /**
     * \brief Called on main thread after the asset has been successfully loaded.
     */
    virtual void OnInitialize() {}

    /**
     * \brief Called on main thread when the asset is starting to load.
     * \param fileName The filename.
     */
    virtual void OnLoadBegin(const std::string& fileName) {}

    /**
     * \brief Similar to OnLoadEnd. Called on main thread after the asset has been successfully loaded.
     * \note After overriding this function, you should call
     * `Asset::OnLoadEnd()` at the end of your custom implementation.
     */
    virtual void OnLoadEnd();

    virtual void OnDeserializeBinary(uint16_t version, BinaryStream& stream) { }
    virtual void OnDeserializeJson(uint16_t version, const json& json) { }
    virtual void OnUnload() = 0;

protected:
    FORCEINLINE virtual AssetBaseType GetAssetBaseType() = 0;
    FORCEINLINE virtual AssetType GetAssetType() = 0;
    FORCEINLINE virtual bool CanLoadAsync()
    {
        return true;
    }

public:
    /// <summary>
    ///     Gets the original asset name.
    /// </summary>
    API_PROPERTY()
    const char* AssetName() const
    {
        return m_assetName.c_str();
    }

    /// <summary>
    ///     Gets source asset file that has been used to load this asset.
    /// </summary>
    API_PROPERTY()
    const char* AssetFile() const
    {
        return m_assetFile.c_str();
    }

    /// <summary>
    ///     Gets asset virtual flag state. True when this asset has been created using CreateVirtualAsset function.
    ///     This means that this asset was never loaded, and has no any representation in content.
    /// </summary>
    API_PROPERTY()
    bool IsVirtual() const
    {
        return m_virtual;
    }

    /**
     * \brief 
     * \return 
     */
    /// <summary>
    ///     Gets the asset load flag state. True when this asset has been loaded or it is virtual asset.
    /// </summary>
    API_PROPERTY()
    virtual bool IsLoaded() const
    {
        return m_loaded || IsVirtual();
    }

public:
    Guid AssetGuid;
};

