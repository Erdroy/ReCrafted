// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ASSET_H
#define ASSET_H

// includes
#include "ReCrafted.h"
#include "Core/Streams/BinaryStream.h"
#include "Scripting/Object.h"

#include "AssetBaseType.h"
#include "AssetType.h"

#include <json.hpp>
using namespace nlohmann;

class Asset : public Object
{
    friend class ContentManager;

private:
    SCRIPTING_API_IMPL();

public:
    static const size_t AssetHeaderSize = 
        3 * sizeof(uint8_t) +   // RCA
        sizeof(uint16_t) +      // Version
        sizeof(uint8_t) +       // Type
        sizeof(Guid::_value);   // Guid

private:
    bool m_virtual = false;
    bool m_registered = false;
    bool m_loaded = false;
    bool m_unloaded = false;

    std::string m_assetName = {};
    std::string m_assetFile = {};

private:
    void OnDestroy() override;

public:
    Asset() = default;
    virtual ~Asset();

protected:
    void Deserialize(BinaryStream& stream);
    void Deserialize(const json& json, const std::string& content);

protected:
    virtual void OnInitialize() {}
    virtual void OnLoadBegin(const std::string& fileName) {}
    virtual void OnLoadEnd() {}
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
    /**
     * \brief Unloads the asset.
     */
    void Unload();

public:
    /**
     * \brief Gets the original asset name.
     * \return The asset name.
     */
    const char* AssetName() const
    {
        return m_assetName.c_str();
    }

    /**
     * \brief Gets source asset file.
     * \return The asset file that has been used to load this asset.
     */
    const char* AssetFile() const
    {
        return m_assetFile.c_str();
    }

    /**
     * \brief Gets asset virtual flag state.
     * \return True when this asset has been created using CreateVirtualAsset function.
     * This means that this asset was never loaded, and has no any representation in content.
     */
    bool IsVirtual() const
    {
        return m_virtual;
    }

    /**
     * \brief Gets the asset load flag state.
     * \return True when this asset has been loaded or it is virtual asset.
     */
    bool IsLoaded() const
    {
        return m_loaded || IsVirtual();
    }

public:
    PROPERTY(Guid, AssetGuid) = {};
};

#endif // ASSET_H