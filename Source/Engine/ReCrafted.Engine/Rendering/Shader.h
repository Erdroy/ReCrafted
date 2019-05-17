// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Content/Assets/JsonAsset.h"
#include "Rendering/Renderer/Renderer.h"
#include "Rendering/Texture.h"

API_USING("ReCrafted.API.Content")

/// <summary>
///     Shader class.
/// </summary>
API_CLASS(public, sealed)
class Shader : public JsonAsset
{
    API_CLASS_BODY()
    friend class Graphics;

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
    ///     Sets texture at given slot.
    /// </summary>
    /// <param name="slot">The texture slot, starts at 0.</param>
    /// <param name="texture">The texture.</param>
    API_FUNCTION()
    void SetTexture(const int slot, Texture* texture)
    {
        Renderer::ApplyTexture2D(texture->GetHandle(), slot);
    }

    /// <summary>
    ///     Sets texture at given slot.
    /// </summary>
    /// <param name="slot">The texture slot, starts at 0.</param>
    /// <param name="texture">The texture.</param>
    void SetTexture(const int slot, const Renderer::Texture2DHandle texture)
    {
        Renderer::ApplyTexture2D(texture, slot);
    }

    /// <summary>
    ///     Gets the shader handle.
    /// </summary>
    /// <returns>The shader handle.</returns>
    Renderer::ShaderHandle GetHandle() const
    {
        return m_shaderHandle;
    }
};
