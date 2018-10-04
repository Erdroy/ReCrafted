// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

// includes
#include "ReCrafted.h"
#include "Content/Assets/BinaryAsset.h"
#include "Renderer/Renderer.hpp"

class Texture : BinaryAsset
{
private:
    Renderer::Texture2DHandle m_textureHandle;

    uint16_t m_width = 0u;
    uint16_t m_height = 0u;
    uint16_t m_format = 0u;
    uint8_t m_mips = 0u;

protected:
    void OnInitialize() override;
    void OnLoadBegin(const std::string& fileName) override;
    void OnDeserializeBinary(uint16_t version, BinaryStream& stream) override;
    void OnUnload() override;
    
    AssetType GetAssetType() override
    {
        return AssetType::Texture;
    }

public:
    void LoadFromMemory(uint8_t* data, uint16_t width, uint16_t height, uint8_t mips, uint8_t bitsPerPixel = 32);
};

#endif // TEXTURE_H
