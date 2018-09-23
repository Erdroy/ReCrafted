// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TESTASSET_H
#define TESTASSET_H

// includes
#include "ReCrafted.h"
#include "AssetBase.h"

class TestAsset : public AssetBase
{
public:
    ~TestAsset() override
    {
        AssetBase::~AssetBase();
    }

protected:
    void OnDeserialize(uint16_t version, BinaryStream& stream) override
    {
        cvar num = stream.ReadUInt64();
        
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
