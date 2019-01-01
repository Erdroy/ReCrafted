// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef BINARYASSET_H
#define BINARYASSET_H

#include "Asset.h"

class BinaryAsset : public Asset
{
protected:
    virtual void OnDeserializeBinary(uint16_t version, BinaryStream& stream) override = 0;

    AssetBaseType GetAssetBaseType() override
    {
        return AssetBaseType::Binary;
    }
};

#endif // BINARYASSET_H
