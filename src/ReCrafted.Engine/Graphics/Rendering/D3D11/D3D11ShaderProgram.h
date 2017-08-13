// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef D3D11SHADERPROGRAM_H
#define D3D11SHADERPROGRAM_H

// includes
#include "D3D11.h"

struct D3D11ShaderProgram
{
private:
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11ComputeShader* m_computeShader = nullptr;

public:
	D3D11ShaderProgram(ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11ComputeShader* cs) : 
	m_vertexShader(vs), 
	m_pixelShader(ps), 
	m_computeShader(cs)
	{
		
	}

public:
	void Release()
	{
		if (m_vertexShader)
			m_vertexShader->Release();
		if (m_pixelShader)
			m_pixelShader->Release();
		if (m_computeShader)
			m_computeShader->Release();

		delete this;
	}
};

D3D11ShaderProgram* LoadShader(const char* fileName);

#endif // D3D11SHADERPROGRAM_H
