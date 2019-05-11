// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Asset.h"

API_CLASS(public, abstract, customNamespace = "ReCrafted.API.Content")
class JsonAsset : public Asset
{
    API_CLASS_BODY()

public:
    virtual ~JsonAsset() = default;

protected:
    virtual void OnDeserializeJson(uint16_t version, const json& json) override = 0;

    AssetBaseType GetAssetBaseType() override
    {
        return AssetBaseType::Json;
    }
};
