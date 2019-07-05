// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Common/Array.h"
#include "Scripting/Object.h"
#include "Rendering/Shader.h"
#include "Rendering/Materials/MaterialField.h"

API_USING("ReCrafted.API.Content")
API_USING("ReCrafted.API.Mathematics")

/// <summary>
///     Material class.
/// </summary>
API_CLASS(public, sealed, partial, customNamespace="ReCrafted.API.Rendering")
class Material final : public JsonAsset
{
    API_CLASS_BODY()

private:
    Shader* m_shader = nullptr;

    List<Texture*> m_textures = {};
    spp::sparse_hash_map<std::string, MaterialField*> m_fields;

protected:
    void OnDeserializeJson(uint16_t version, const json& json) override;
    void OnUnload() override;

    AssetType GetAssetType() override
    {
        return AssetType::Material;
    }

    bool CanLoadAsync() override
    {
        return true;
    }

public:
    // TODO: Runtime MaterialField editing. (Make sure that this asset is loaded)
    //void SetValue(const char* name) { }

public:
    /// <summary>
    ///     Gets the shader that this material uses.
    /// </summary>
    API_PROPERTY(noprefix)
    Shader* GetShader() const
    {
        return m_shader;
    }
};