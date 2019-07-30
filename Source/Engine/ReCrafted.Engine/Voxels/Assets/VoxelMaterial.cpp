// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelMaterial.h"
#include "Content/ContentManager.h"

void VoxelMaterial::OnLoadEnd()
{
    // Load textures
    m_voxelTextureCB = ContentManager::LoadAsset<Texture>(m_textureAssetCB.c_str());

    if (m_hasNormalTexture)
        m_voxelTextureNSM = ContentManager::LoadAsset<Texture>(m_textureAssetNSM.c_str());

    if (m_hasFarTextures)
        m_voxelTextureCBFar = ContentManager::LoadAsset<Texture>(m_textureAssetCBFar.c_str());

    if (m_hasNormalTexture && m_hasFarTextures)
        m_voxelTextureNSMFar = ContentManager::LoadAsset<Texture>(m_textureAssetNSMFar.c_str());
}

void VoxelMaterial::OnUnload()
{
    ContentManager::UnloadAssetSafe(m_voxelTextureCB);
    ContentManager::UnloadAssetSafe(m_voxelTextureNSM);
    ContentManager::UnloadAssetSafe(m_voxelTextureCBFar);
    ContentManager::UnloadAssetSafe(m_voxelTextureNSMFar);

    // Try to unregister this material
    VoxelMaterialManager::GetInstance()->UnregisterMaterial(this);
}

void VoxelMaterial::OnDeserializeJson(uint16_t version, const json& json)
{
    m_voxelName = json["VoxelName"].get<std::string>();
    m_voxelMaterial = json["VoxelMaterial"].get<VoxelMaterial_t>();
    m_voxelHardness = json["VoxelHardness"].get<VoxelHardness_t>();

    ASSERT(json["VoxelTexture_CB"].is_null() == false);

    // Load all texture names
    m_textureAssetCB = json["VoxelTexture_CB"].get<std::string>();

    if (!json["VoxelTexture_NSM"].is_null())
    {
        m_textureAssetNSM = json["VoxelTexture_NSM"].get<std::string>();
        m_hasNormalTexture = true;
    }

    if (!json["VoxelTexture_CB_Far"].is_null())
    {
        m_textureAssetCBFar = json["VoxelTexture_CB_Far"].get<std::string>();
        m_hasFarTextures = true;
    }

    if (m_hasNormalTexture && m_hasFarTextures)
    {
        _ASSERT_(json["VoxelTexture_NSM_Far"].is_null(),
            "When standard normal texture is provided and CB far texture, the NSM texture is also required!");

        m_textureAssetNSMFar = json["VoxelTexture_NSM_Far"].get<std::string>();
    }
}
