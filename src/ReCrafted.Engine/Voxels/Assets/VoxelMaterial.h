// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELMATERIAL_H
#define VOXELMATERIAL_H

// includes
#include "Content/Assets/JsonAsset.h"
#include "Graphics/Texture2D.h"
#include "Voxels/Voxel.h"
#include "Voxels/VoxelMaterialManager.h"

class VoxelMaterial : public JsonAsset
{
private:
    std::string m_voxelName = nullptr;
    VoxelMaterial_t m_voxelMaterial = 0u;
    VoxelHardness_t m_voxelHardness = 0u;
    Texture2D* m_voxelTexture = nullptr;

    std::string m_textureAsset = nullptr;

protected:
    void OnInitialize() override
    {
        
    }

    void OnLoadEnd() override
    {
        // TODO: load texture

        // Register this material
        VoxelMaterialManager::GetInstance()->RegisterMaterial(this);
    }

    void OnUnload() override
    {
        VoxelMaterialManager::GetInstance()->UnregisterMaterial(this);
    }
    
    void OnDeserializeJson(uint16_t version, const json& json) override
    {
        m_voxelMaterial = json["VoxelMaterial"].get<VoxelMaterial_t>();
        m_voxelHardness = json["VoxelHardness"].get<VoxelHardness_t>();
        m_textureAsset = json["VoxelTexture"].get<std::string>();
        m_voxelName = json["VoxelName"].get<std::string>();
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

    Texture2D* GetTexture() const
    {
        return m_voxelTexture;
    }
};

#endif // VOXELMATERIAL_H
