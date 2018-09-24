// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef JSONASSET_H
#define JSONASSET_H

#include "Asset.h"

class JsonAsset : public Asset
{
public:
    virtual void OnDeserializeJson(uint16_t version, const json& json) override = 0;

    AssetBaseType GetAssetBaseType() override
    {
        return AssetBaseType::Json;
    }
};

#endif // JSONASSET_H
