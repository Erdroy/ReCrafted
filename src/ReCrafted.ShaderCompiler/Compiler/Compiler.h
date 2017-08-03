// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef COMPILER_H
#define COMPILER_H

class Compiler
{
private:
	Compiler() {}

public:
	/**
	 * \brief Compiles shader using given parameters.
	 * \param input The input HLSL (ReCrafted subset) shader file.
	 * \param output The output shader file.
	 * \param profile The shader target profile (d3d11 only).
	 * \param dx Generate bytecode for D3D11?
	 * \param gl Generate bytecode for OpenGL 3.5+?
	 * \param vlk Generate bytecode Vulkan?
	 * \return True when succeeded.
	 */
	static bool compile(const char* input, const char* output, const char* profile, bool dx, bool gl, bool vlk);
};

#endif // COMPILER_H
