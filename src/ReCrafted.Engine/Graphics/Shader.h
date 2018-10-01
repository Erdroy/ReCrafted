// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SHADER_H
#define SHADER_H

// includes
#include "ReCrafted.h"
#include "Core/Types.h"
#include "Texture2D.h"
#include "Renderer/Renderer.hpp"

#include "Content/Assets/JsonAsset.h"

/**
 * \brief Shader class.
 */
class Shader : public JsonAsset
{
    friend class Graphics;
    friend class UI;

private:
    Renderer::ShaderHandle m_shaderHandle;
    Text m_shaderName;

public:
    void OnInitialize() override;
    void OnLoadBegin(const std::string& fileName) override;
    void OnDeserializeJson(uint16_t version, const json& json) override;
    void OnUnload() override;

    AssetType GetAssetType() override
    {
        return AssetType::Shader;
    }

public:
    /**
	 * \brief Sets value of given type and value to a given slot.
	 * \tparam T Value type.
	 * \param slot Slot, starts at 0.
	 * \param value The value.
	 */
    template <class T>
    void SetValue(uint slot, T* value)
    {
        Renderer::SetShaderValue(m_shaderHandle, 0, slot, value, sizeof(T));
    }

    /**
	 * \brief Sets texture at given slot.
	 * \param slot The texture slot, starts at 0.
	 * \param texture The texture.
	 */
    void SetTexture(int slot, RefPtr<Texture2D> texture)
    {
        Renderer::ApplyTexture2D(texture->m_textureHandle, slot);
    }

    /**
    * \brief Sets texture at given slot.
    * \param slot The texture slot, starts at 0.
    * \param texture The texture.
    */
    void SetTexture(int slot, Renderer::Texture2DHandle texture)
    {
        Renderer::ApplyTexture2D(texture, slot);
    }

    /**
     * \brief Gets the shader handle.
     * \return The shader handle.
     */
    Renderer::ShaderHandle GetHandle() const
    {
        return m_shaderHandle;
    }
};

#endif // SHADER_H
