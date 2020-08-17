// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Array.h"
#include "Content/Assets/JsonAsset.h"
#include "Rendering/Renderer/Renderer.h"

API_USING("ReCrafted.API.Content")

/// <summary>
///     Shader class.
/// </summary>
API_CLASS(public, sealed)
class Shader : public JsonAsset
{
    API_CLASS_BODY();
    ASSET_BODY(Shader);

private:
    Renderer::ShaderHandle m_shaderHandle;
    String m_shaderName;

protected:
    void OnInitialize() override;
    void OnLoadBegin(const std::string& fileName) override;
    void OnDeserializeJson(uint16_t version, const json& json) override;
    void OnUnload() override;

    AssetType GetAssetType() override
    {
        return AssetType::Shader;
    }

    bool CanLoadAsync() override
    {
        return false; // Just because we are not going to use shader JSON directly, but renderer will do it
    }

public:
    /// <summary>
    ///     Sets value of given type and value to a given slot.
    /// </summary>
    /// <typeparam name="T">Value type.</typeparam>
    /// <param name="slot">Slot, starts at 0.</param>
    /// <param name="value">The value.</param>
    template <class T>
    void SetValue(const uint slot, T* value)
    {
        Renderer::SetShaderValue(m_shaderHandle, 0, slot, value, sizeof(T));
    }

    /// <summary>
    ///     Sets texture array to a given slot.
    /// </summary>
    /// <param name="slot">The texture array slot, starts at 0.</param>
    /// <param name="textureArray">The texture array.</param>
    API_FUNCTION()
    void SetTextureArray(const int slot, const Array<Texture*>& textureArray);

    /// <summary>
    ///     Sets texture at given slot.
    /// </summary>
    /// <param name="slot">The texture slot, starts at 0.</param>
    /// <param name="texture">The texture.</param>
    API_FUNCTION()
    void SetTexture(const int slot, Texture* texture);

    /// <summary>
    ///     Sets texture at given slot.
    /// </summary>
    /// <param name="slot">The texture slot, starts at 0.</param>
    /// <param name="texture">The texture.</param>
    void SetTexture(const int slot, const Renderer::Texture2DHandle texture);

    /// <summary>
    ///     Gets the shader handle.
    /// </summary>
    /// <returns>The shader handle.</returns>
    Renderer::ShaderHandle GetHandle() const
    {
        return m_shaderHandle;
    }
};
