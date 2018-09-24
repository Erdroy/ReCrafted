// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TESTASSET_H
#define TESTASSET_H

// includes
#include "ReCrafted.h"
#include "BinaryAsset.h"
#include "JsonAsset.h"

class TestAsset : public JsonAsset
{
protected:
    /*void OnDeserializeBinary(uint16_t version, BinaryStream& stream) override
    {
        cvar num = stream.ReadUInt64();
        
    }
    */
    void OnDeserializeJson(uint16_t version, const json& json) override
    {
        
    }

    void OnUnload() override
    {
        
    }

public:
    FORCEINLINE AssetType GetAssetType() override
    {
        return AssetType::Test;
    }
};

#endif // TESTASSET_H
