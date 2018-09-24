// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ASSET_H
#define ASSET_H

// includes
#include "ReCrafted.h"
#include "Core/Streams/BinaryStream.h"

#include "AssetBaseType.h"
#include "AssetType.h"

#include <json.hpp>
using namespace nlohmann;

class Asset
{
    friend class ContentManager;

private:
    bool m_unloaded = false;

public:
    Asset() = default;
    virtual ~Asset();

public:
    void Deserialize(BinaryStream& stream);
    void Deserialize(const json& json, const std::string& content);
    void Unload();

protected:
    virtual void OnInitialize() {}
    virtual void OnDeserializeBinary(uint16_t version, BinaryStream& stream) { }
    virtual void OnDeserializeJson(uint16_t version, const json& json) { }
    virtual void OnUnload() = 0;

public:
    FORCEINLINE virtual AssetBaseType GetAssetBaseType() = 0;
    FORCEINLINE virtual AssetType GetAssetType() = 0;

public:
    PROPERTY(Guid, AssetGuid) = {};
};

#endif // ASSET_H