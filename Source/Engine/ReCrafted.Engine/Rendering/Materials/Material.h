// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Content/ContentManager.h"
#include "Content/Assets/JsonAsset.h"
#include "Scripting/Object.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/Materials/MaterialField.h"

API_USING("ReCrafted.API.Content")
API_USING("ReCrafted.API.Mathematics")

/// <summary>
///     Material class.
/// </summary>
API_CLASS(public, sealed, partial, customNamespace="ReCrafted.API.Rendering")
class Material final : public JsonAsset
{
    API_CLASS_BODY();
    ASSET_BODY(Material);

    friend class ModelRenderingSystem;

private:
    std::string m_name = {};
    Shader* m_shader = nullptr;
    uint8_t m_shaderPass = 0u;
    List<List<Texture*>> m_textureArrays = {};
    List<Texture*> m_textures = {};
    List<MaterialField*> m_fields = {};

    // Loading helpers
    std::string m_shaderAssetName = {};
    List<std::string> m_textureLoadQueue = {};
    List<List<std::string>> m_textureArrayLoadQueue = {};

private:
    static MaterialFieldType ParseFieldType(const std::string& type);

protected:
    void OnDeserializeJson(uint16_t version, const json& json) override;
    void OnLoadEnd() override;
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

    void SetTextureArray(const int slot, const List<Texture*>& textureArray)
    {
        if(m_textureArrays.Count() == 0)
            m_textureArrays.Add({});

        m_textureArrays[slot].Clear();
        m_textureArrays[slot].Copy(textureArray);
    }

public:
    /// <summary>
    ///     Gets or sets the material name.
    /// </summary>
    API_PROPERTY()
    void MaterialName(const char* name)
    {
        ASSERT(name != nullptr);
        m_name = name;
    }

    /// <summary>
    ///     Gets or sets the material name.
    /// </summary>
    API_PROPERTY()
    const char* MaterialName() const
    {
        return m_name.c_str();
    }

    /// <summary>
    ///     Gets the shader that this material uses.
    /// </summary>
    API_PROPERTY(noprefix)
    Shader* GetShader() const
    {
        return m_shader;
    }
};