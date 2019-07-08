// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Material.h"
#include "Content/ContentManager.h"

void Material::OnDeserializeJson(uint16_t version, const json& json)
{
    // Setup material name
    const auto mName = json["MaterialName"];
    ASSERT(!mName.is_null());
    ASSERT(mName.is_string());
    m_name = mName.get<std::string>();

    // Setup shader asset file
    const auto mShader = json["MaterialShader"];
    ASSERT(!mShader.is_null());
    ASSERT(mShader.is_string());
    m_shaderAssetName = mShader.get<std::string>();

    // Load textures
    for(auto&& textureArrayItem : json["Textures"])
    {
        // Do not allow non-specified asset names
        // This would throw exception in the ContentManager and also adding null-texture would
        // end with an undefined-behaviour in the rendering pipeline (but more likely would end with an exception).
        ASSERT(!textureArrayItem.is_null());
        ASSERT(textureArrayItem.is_string());

        // Get texture asset name
        const auto assetName = textureArrayItem.get<std::string>();
        ASSERT(!assetName.empty());

        // We cannot use the ContentManager here, because we are running on task worker thread.
        // There is no need to lock the texture queue list as it will not be accessed by any other thread at this moment.
        m_textureLoadQueue.Add(assetName);
    }

    // TODO: Load fields
}

void Material::OnLoadEnd()
{
    // Load shader
    ASSERT(!m_shaderAssetName.empty());
    m_shader = ContentManager::LoadAsset<Shader>(m_shaderAssetName.c_str());
    m_shaderAssetName.clear();

    // Load textures
    for(auto&& textureAssetName : m_textureLoadQueue)
    {
        ASSERT(!textureAssetName.empty());

        // Load texture (this will start loading the texture), at first, we will get only first mip.
        const auto textureAsset = ContentManager::LoadAsset<Texture>(textureAssetName.c_str());

        // Make sure that we have got the asset.
        ASSERT(textureAsset);

        m_textures.Add(textureAsset);
    }

    // Clear texture load queue
    m_textureLoadQueue.Clear();
}

void Material::OnUnload()
{
    // Destroy textures
    for(auto&& texture : m_textures)
        Destroy(texture);
    m_textures.Clear();

    // TODO: Release material fields
}
