// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SHADER_H
#define SHADER_H

// includes
#include "ReCrafted.h"
#include "Core/Types.h"
#include "Texture2D.h"

/**
 * \brief Shader class.
 */
class Shader : IResource
{
	friend class Graphics;
	friend class UI;

private:
	char m_shaderName[128] = {};

	uint m_uniformCount = 0u;

public:
    IRESOURCE_IMPL(Shader)

private:
	void init(const char* vs, const char* fs, const char* def);

public:
    /**
	 * \brief Sets value of given type and value to a given slot.
	 * \tparam T Value type.
	 * \param slot Slot, starts at 0.
	 * \param value The value.
	 */
	template<class T>
	void setValue(uint slot, T* value)
	{
		//bgfx::setUniform(m_uniforms[slot], value);
	}

    /**
	 * \brief Sets texture at given slot.
	 * \param slot The texture slot, starts at 0.
	 * \param texture The texture.
	 */
	void setTexture(int slot, Ptr<Texture2D> texture);

    /**
	 * \brief Gets BGFX shader program handle.
	 * \return The BGFX shader program handle.
	 */
	//bgfx::ProgramHandle getProgram();

    /**
	 * \brief Disposes this shader.
	 */
	void dispose() override;

public:
    /**
	 * \brief Loads shader by name.
	 * \param shaderFile The shader name, eg.: unlit/unlit_color
	 * \return The loaded shader, or nullptr when file not found.
	 */
	static Ptr<Shader> loadShader(const char* shaderFile);
};

#endif // SHADER_H