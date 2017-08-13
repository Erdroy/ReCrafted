// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "D3D11ShaderProgram.h"

#include "../../../../ReCrafted.ShaderCompiler/Compiler/ShaderMeta.h"
#include "../../../../ReCrafted.ShaderCompiler/Compiler/File.h"
#include "D3D11Renderer.h"

typedef enum // from `hlslcc.h`
{
	INVALID_SHADER = -1,
	PIXEL_SHADER,
	VERTEX_SHADER,
	GEOMETRY_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	COMPUTE_SHADER,
} SHADER_TYPE;

D3D11ShaderProgram* LoadShader(const char* fileName)
{
	auto m_device = static_cast<ID3D11Device*>(D3D11Renderer::getDevice());

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11ComputeShader* computeShader = nullptr;

	File shaderFile = {};
	File::openFile(&shaderFile, fileName, OpenMode::OpenRead);

	shadermeta meta = {};
	meta.read(shaderFile);

	auto passCount = shaderFile.read<int>();

	for(auto passId = 0; passId < passCount; passId ++)
	{
		auto pass = meta.passes[passId];

		auto shaderCount = shaderFile.read<int>();

		// create shaders
		for (auto shaderId = 0; shaderId < shaderCount; shaderId++)
		{
			auto shaderType = static_cast<SHADER_TYPE>(shaderFile.read<int>());
			
			// read HLSL
			auto hlsl_len = shaderFile.read<int>();
			if (hlsl_len > 1024 * 64) throw;

			auto hlsl_data = new char[hlsl_len];
			shaderFile.read(hlsl_data, hlsl_len);

			// we now have the shader bytecode
			// create shader
			switch(shaderType)
			{
			case PIXEL_SHADER: 
				m_device->CreatePixelShader(hlsl_data, hlsl_len, nullptr, &pixelShader);
				break;
			case VERTEX_SHADER:
				m_device->CreateVertexShader(hlsl_data, hlsl_len, nullptr, &vertexShader);
				// TODO: try to build input layout
				break;
			case COMPUTE_SHADER:
				m_device->CreateComputeShader(hlsl_data, hlsl_len, nullptr, &computeShader);
				break;
			default: break;
			}

			delete[] hlsl_data;

			// read GLSL
			auto glsl_len = shaderFile.read<int>();
			if (glsl_len > 1024 * 64) throw;

			auto glsl_data = new char[glsl_len];
			shaderFile.read(glsl_data, glsl_len);

			// we don't need GLSL, but we need to push the stream

			delete[] glsl_data;
		}
	}

	auto sp = new D3D11ShaderProgram(vertexShader, pixelShader, computeShader);

	// create buffers
	// TODO: create buffers

	return sp;
}
