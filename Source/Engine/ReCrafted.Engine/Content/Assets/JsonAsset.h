// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Asset.h"

class JsonAsset : public Asset
{
protected:
    virtual void OnDeserializeJson(uint16_t version, const json& json) override = 0;

    AssetBaseType GetAssetBaseType() override
    {
        return AssetBaseType::Json;
    }
};
