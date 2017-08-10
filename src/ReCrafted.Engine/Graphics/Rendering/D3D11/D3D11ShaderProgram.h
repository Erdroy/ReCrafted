// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef D3D11SHADERPROGRAM_H
#define D3D11SHADERPROGRAM_H

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

D3D11ShaderProgram* LoadShader(const char* fileName);

#endif // D3D11SHADERPROGRAM_H
