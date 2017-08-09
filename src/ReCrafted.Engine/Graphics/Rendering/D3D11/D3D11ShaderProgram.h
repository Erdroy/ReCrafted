// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef D3D11SHADERPROGRAM_H
#define D3D11SHADERPROGRAM_H

// includes
#include "../../../../ReCrafted.ShaderCompiler/Compiler/Compiler.h"

struct D3D11ShaderProgram
{
public:
	// TODO: impl

public:
	void Release()
	{
		delete this;
	}
};

inline D3D11ShaderProgram* LoadShader(const char* fileName)
{
	auto sp = new D3D11ShaderProgram;

	File shaderFile = {};
	File::openFile(&shaderFile, fileName, OpenMode::OpenRead);

	shadermeta meta = {};
	meta.read(shaderFile);

	return sp;
}

#endif // D3D11SHADERPROGRAM_H
