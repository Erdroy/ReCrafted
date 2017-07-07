// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef SHADER_H
#define SHADER_H

// includes
#include "Utils/Types.h"
#include "Texture2D.h"

struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix;

/// <summary>
/// Shader class.
/// </summary>
class Shader
{
	friend class Rendering;
	friend class UI;

private:
	char m_shaderName[128] = {};

	bgfx::ShaderHandle m_vertexshader = {};
	bgfx::ShaderHandle m_fragmentshader = {};
	bgfx::ProgramHandle m_program = {};
	bgfx::UniformHandle m_uniforms[16] = {};
	bgfx::UniformHandle m_textures[16] = {};

	uint m_uniformCount = 0u;

private:
	void init(const char* vs, const char* fs, const char* def);

public:

	/// <summary>
	/// Set value.
	/// </summary>
	/// <param name="slot">Slot, starts at 0.</param>
	/// <param name="value">The value.</param>
	template<class T>
	void setValue(uint slot, T* value)
	{
		bgfx::setUniform(m_uniforms[slot], value);
	}

	void setTexture(int slot, Ptr<Texture2D> texture);

	/// <summary>
	/// Disposes this shader.
	/// </summary>
	void dispose();

public:
	/// <summary>
	/// Load shader by name.
	/// </summary>
	/// <param name="shaderFile">The shader name, eg.: unlit/unlit_color</param>
	/// <returns>The loaded shader, or null when file not found.</returns>
	static Ptr<Shader> loadShader(const char* shaderFile);
};

#endif // SHADER_H