// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef AUDIOSOUND_H
#define AUDIOSOUND_H

// includes
#include "ReCrafted.h"
#include "Content/Assets/BinaryAsset.h"

class AudioSound : public BinaryAsset
{
private:
    SCRIPTING_API_IMPL();

public:
    AudioSound() = default;

protected:
    void OnInitialize() override;
    void OnUnload() override;
    void OnDeserializeBinary(uint16_t version, BinaryStream& stream) override;

    AssetType GetAssetType() override
    {
        return AssetType::Unknown;
    }
};

#endif // AUDIOSOUND_H
