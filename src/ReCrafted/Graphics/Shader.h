// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef SHADER_H
#define SHADER_H

// includes
#include "../Utils/Types.h"
#include "../Utils/Defines.h"
#include <bgfx/bgfx.h>

class Shader
{
	friend class Rendering;

private:
	bgfx::ShaderHandle m_vertexshader = {};
	bgfx::ShaderHandle m_fragmentshader = {};

	bgfx::ProgramHandle m_program = {};

private:
	void init(const char* vs, const char* fs);

public:

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