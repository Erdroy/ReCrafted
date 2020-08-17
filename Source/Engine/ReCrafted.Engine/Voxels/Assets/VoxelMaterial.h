// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Content/Assets/JsonAsset.h"
#include "Rendering/Texture.h"
#include "Voxels/Voxel.h"
#include "Voxels/VoxelMaterialManager.h"

API_USING("ReCrafted.API.Content");
API_USING("ReCrafted.API.Rendering");

API_CUSTOM_TRANSLATION(VoxelMaterial_t, "ushort")
API_CUSTOM_TRANSLATION(VoxelHardness_t, "sbyte")

enum class VoxelMaterialType
{
    ColorBlend,
    NormalSmoothnessMetallic,
    ColorBlendFar,
    NormalSmoothnessMetallicFar
};

API_CLASS(public, sealed, customNamespace = "ReCrafted.API.Voxels")
class VoxelMaterial final : public JsonAsset
{
    API_CLASS_BODY();

private:
    std::string m_voxelName;

    VoxelMaterial_t m_voxelMaterial = 0u;
    VoxelHardness_t m_voxelHardness = 0u;

    Texture* m_voxelTextureCB = nullptr;
    Texture* m_voxelTextureNSM = nullptr;
    Texture* m_voxelTextureCBFar = nullptr;
    Texture* m_voxelTextureNSMFar = nullptr;

    std::string m_textureAssetCB;
    std::string m_textureAssetNSM;
    std::string m_textureAssetCBFar;
    std::string m_textureAssetNSMFar;

    bool m_hasFarTextures = false;
    bool m_hasNormalTexture = false;

protected:
    void OnLoadEnd() override;
    void OnUnload() override;

    void OnDeserializeJson(uint16_t version, const json& json) override;

    AssetType GetAssetType() override
    {
        return AssetType::VoxelMaterial;
    }

public:
    API_PROPERTY(noprefix)
    const char* GetName() const
    {
        return m_voxelName.c_str();
    }

    API_PROPERTY(noprefix)
    VoxelMaterial_t GetMaterial() const
    {
        return m_voxelMaterial;
    }

    API_PROPERTY(noprefix)
    VoxelHardness_t GetHardness() const
    {
        return m_voxelHardness;
    }

    API_PROPERTY()
    bool HasNormalTextures() const
    {
        return m_hasNormalTexture;
    }

    API_PROPERTY()
    bool HasFarTextures() const
    {
        return m_hasFarTextures;
    }

    API_FUNCTION()
    Texture* GetTexture(const VoxelMaterialType type) const
    {
        switch (type)
        {
        case VoxelMaterialType::ColorBlend:
            return m_voxelTextureCB;
        case VoxelMaterialType::NormalSmoothnessMetallic:
            return m_voxelTextureNSM;
        case VoxelMaterialType::ColorBlendFar:
            return m_voxelTextureCBFar;
        case VoxelMaterialType::NormalSmoothnessMetallicFar:
            return m_voxelTextureNSMFar;
        default:
            return nullptr;
        }
    }
};
