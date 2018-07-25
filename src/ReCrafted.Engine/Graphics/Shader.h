// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SHADER_H
#define SHADER_H

// includes
#include "ReCrafted.h"
#include "Core/Types.h"
#include "Texture2D.h"
#include "Renderer/Renderer.hpp"

/**
 * \brief Shader class.
 */
class Shader : IResource
{
    friend class Graphics;
    friend class UI;

private:
    Renderer::ShaderHandle m_shaderHandle;
    Text m_shaderName;

public:
IRESOURCE_IMPL(Shader)

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
	 * \brief Disposes this shader.
	 */
    void Dispose() override;

    /**
     * \brief Gets the shader handle.
     * \return The shader handle.
     */
    Renderer::ShaderHandle GetHandle() const
    {
        return m_shaderHandle;
    }

public:
    /**
	 * \brief Loads shader by name.
	 * \param shaderName The shader name, eg.: GBufferStandard etc.
	 * \return The loaded shader, or nullptr when file not found.
	 */
    static RefPtr<Shader> LoadShader(const char* shaderName);
};

#endif // SHADER_H
