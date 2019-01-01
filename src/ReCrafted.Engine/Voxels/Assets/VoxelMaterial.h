// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef VOXELMATERIAL_H
#define VOXELMATERIAL_H

// includes
#include "Content/Assets/JsonAsset.h"
#include "Graphics/Texture.h"
#include "Voxels/Voxel.h"
#include "Voxels/VoxelMaterialManager.h"
#include "Content/ContentManager.h"

class VoxelMaterial : public JsonAsset
{
private:
    SCRIPTING_API_IMPL();

public:
    enum class VoxelMaterialType
    {
        ColorBlend,
        NormalSmoothnessMetallic,
        ColorBlendFar,
        NormalSmoothnessMetallicFar
    };

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
    void OnLoadEnd() override
    {
        // Load textures
        m_voxelTextureCB = ContentManager::LoadAsset<Texture>(m_textureAssetCB.c_str());
        
        if(m_hasNormalTexture)
            m_voxelTextureNSM = ContentManager::LoadAsset<Texture>(m_textureAssetNSM.c_str());
        
        if(m_hasFarTextures)
            m_voxelTextureCBFar = ContentManager::LoadAsset<Texture>(m_textureAssetCBFar.c_str());

        if(m_hasNormalTexture && m_hasFarTextures)
            m_voxelTextureNSMFar = ContentManager::LoadAsset<Texture>(m_textureAssetNSMFar.c_str());
    }

    void OnUnload() override
    {
        ContentManager::UnloadAssetSafe(m_voxelTextureCB);
        ContentManager::UnloadAssetSafe(m_voxelTextureNSM);
        ContentManager::UnloadAssetSafe(m_voxelTextureCBFar);
        ContentManager::UnloadAssetSafe(m_voxelTextureNSMFar);

        // Try to unregister this material
        VoxelMaterialManager::GetInstance()->UnregisterMaterial(this);
    }
    
    void OnDeserializeJson(uint16_t version, const json& json) override
    {
        m_voxelName = json["VoxelName"].get<std::string>();
        m_voxelMaterial = json["VoxelMaterial"].get<VoxelMaterial_t>();
        m_voxelHardness = json["VoxelHardness"].get<VoxelHardness_t>();

        ASSERT(json["VoxelTexture_CB"].is_null() == false);

        // Load all texture names
        m_textureAssetCB = json["VoxelTexture_CB"].get<std::string>();

        if(!json["VoxelTexture_NSM"].is_null())
        {
            m_textureAssetNSM = json["VoxelTexture_NSM"].get<std::string>();
            m_hasNormalTexture = true;
        }

        if (!json["VoxelTexture_CB_Far"].is_null())
        {
            m_textureAssetCBFar = json["VoxelTexture_CB_Far"].get<std::string>();
            m_hasFarTextures = true;
        }

        if(m_hasNormalTexture && m_hasFarTextures)
        {
            _ASSERT_(json["VoxelTexture_NSM_Far"].is_null(), 
                "When standard normal texture is provided and CB far texture, the NSM texture is also required!");

            m_textureAssetNSMFar = json["VoxelTexture_NSM_Far"].get<std::string>();
        }
    }

    const char* GetObjectName() const override
    {
        return "(asset) VoxelMaterial";
    }

    AssetType GetAssetType() override
    {
        return AssetType::VoxelMaterial;
    }

public:
    const std::string& GetName() const
    {
        return m_voxelName;
    }

    VoxelMaterial_t GetMaterial() const
    {
        return m_voxelMaterial;
    }

    VoxelHardness_t GetHardness() const
    {
        return m_voxelHardness;
    }

    bool HasNormalTextures() const
    {
        return m_hasNormalTexture;
    }

    bool HasFarTextures() const
    {
        return m_hasFarTextures;
    }

    Texture* GetTexture(const VoxelMaterialType type) const
    {
        switch(type)
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

#endif // VOXELMATERIAL_H
