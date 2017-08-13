// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef D3D11SHADERPROGRAM_H
#define D3D11SHADERPROGRAM_H

// includes
#include "D3D11.h"

#include <string>
#include <vector>
#include <algorithm>

struct D3D11ShaderPass
{
public:
	std::string name = {};

	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11ComputeShader* m_computeShader = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;

	void Release()
	{
		if (m_vertexShader)
			m_vertexShader->Release();
		if (m_pixelShader)
			m_pixelShader->Release();
		if (m_computeShader)
			m_computeShader->Release();

		// TODO: proper input layout release
	}
};

struct D3D11ShaderProgram
{
private:
	std::vector<D3D11ShaderPass> m_passes = {};

public:
	D3D11ShaderProgram(std::vector<D3D11ShaderPass> shaderPasses)
	{
		m_passes = shaderPasses;
	}

public:
	void Apply(const char* pass_name);

	void Release()
	{
		// release all passes
		for(auto & pass : m_passes)
			pass.Release();

		// suicide
		delete this;
	}
};

D3D11ShaderProgram* LoadShader(const char* fileName);

#endif // D3D11SHADERPROGRAM_H
