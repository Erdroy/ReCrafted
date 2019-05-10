// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Asset.h"

API_CLASS(public, abstract, customNamespace = "ReCrafted.API.Content")
class BinaryAsset : public Asset
{
    API_CLASS_BODY()

protected:
    virtual void OnDeserializeBinary(uint16_t version, BinaryStream& stream) override = 0;

    AssetBaseType GetAssetBaseType() override
    {
        return AssetBaseType::Binary;
    }
};
