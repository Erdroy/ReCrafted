// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef D3D11SHADERPROGRAM_H
#define D3D11SHADERPROGRAM_H

// includes
#include "D3D11.h"

#include <vector>
#include <string>

struct D3D11ShaderPass
{
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

struct D3D11ShaderBufferField
{
	std::string name = {};

	uint8_t size = 0;
	uint16_t offset = 0;
};

struct D3D11ShaderBuffer
{
public:
	ID3D11Buffer* m_buffer = nullptr;
	byte* m_data = nullptr;
	bool m_dirty = false;
	uint16_t m_size = 0u;

public:
	std::string name = {};
	std::vector<D3D11ShaderBufferField> fields = {};

	void Release()
	{
		if (m_buffer)
			m_buffer->Release();

		if(m_data)
			delete[] m_data;
	}
};

struct D3D11ShaderProgram
{
private:
	std::vector<D3D11ShaderPass> m_passes = {};
	std::vector<D3D11ShaderBuffer> m_buffers = {};

	std::vector<ID3D11Buffer*> m_buffersCache = {}; // for fast buffer bind

private:
	void updateBuffers(int buffer_index);

public:
	D3D11ShaderProgram(const std::vector<D3D11ShaderPass>& shader_passes, const std::vector<D3D11ShaderBuffer>& shader_buffers)
		: m_passes(shader_passes),
		  m_buffers(shader_buffers)
	{
		for(auto & buffer : shader_buffers)
		{
			m_buffersCache.push_back(buffer.m_buffer);
		}
	}

	void Apply(const char* pass_name);

	void Apply(int pass_index);

	void SetValue(const char* buffer_name, const char* field_name, void* value);

	void SetValue(int buffer_index, int field_index, void* value);

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
