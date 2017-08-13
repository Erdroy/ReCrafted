// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "D3D11ShaderProgram.h"

#include "../../../../ReCrafted.ShaderCompiler/Compiler/ShaderMeta.h"
#include "../../../../ReCrafted.ShaderCompiler/Compiler/File.h"

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
	auto sp = new D3D11ShaderProgram;

	File shaderFile = {};
	File::openFile(&shaderFile, fileName, OpenMode::OpenRead);

	shadermeta meta = {};
	meta.read(shaderFile);

	auto passCount = shaderFile.read<int>();

	for(auto passId = 0; passId < passCount; passId ++)
	{
		auto pass = meta.passes[passId];

		auto shaderCount = shaderFile.read<int>();

		for (auto shaderId = 0; shaderId < shaderCount; shaderId++)
		{
			auto shaderType = static_cast<SHADER_TYPE>(shaderFile.read<int>());
			
			auto d3d_len = shaderFile.read<int>();
			if (d3d_len > 1024 * 64) throw;

			auto d3d_data = new char[d3d_len];

			shaderFile.read(d3d_data, d3d_len);

			// skip glsl
			auto glsl_len = shaderFile.read<int>();
			shaderFile.read(d3d_data, glsl_len);

			delete [] d3d_data;
		}
	}

	return sp;
}
