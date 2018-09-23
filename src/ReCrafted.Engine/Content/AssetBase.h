// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ASSETBASE_H
#define ASSETBASE_H

// includes
#include "ReCrafted.h"
#include "Core/Streams/BinaryStream.h"

#include "AssetType.h"

class AssetBase
{
    friend class ContentManager;

private:
    bool m_unloaded = false;

public:
    AssetBase() = default;
    virtual ~AssetBase();

public:
    void Deserialize(BinaryStream& stream);
    void Unload();

protected:
    virtual void OnInitialize() {}
    virtual void OnDeserialize(uint16_t version, BinaryStream& stream) = 0;
    virtual void OnUnload() = 0;

public:
    FORCEINLINE virtual AssetType GetAssetType() = 0;

public:
    PROPERTY(Guid, AssetGuid) = {};
};

#endif // ASSETBASE_H