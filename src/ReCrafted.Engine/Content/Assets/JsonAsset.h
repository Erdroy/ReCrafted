// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef JSONASSET_H
#define JSONASSET_H

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

#endif // JSONASSET_H
