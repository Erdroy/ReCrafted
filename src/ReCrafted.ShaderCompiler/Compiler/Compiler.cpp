// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#define _CRT_SECURE_NO_WARNINGS

#include "Compiler.h"

#include "ShaderMeta.h"
#include "Parser.h"
#include "File.h"

#include <hlslcc.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

#if _DEBUG
#pragma comment(lib, "hlslccd.lib")
#else
#pragma comment(lib, "hlslcc.lib")
#endif

char* readfile(const char* input, int* filesize)
{
	// load file
	auto file = fopen(input, "r");

	if (file == nullptr)
	{
		printf("Error: input HLSL shader file not found!\n");
		return nullptr;
	}

	// get file size
	fseek(file, 0, SEEK_END);
	*filesize = ftell(file) + 1;
	fseek(file, 0, SEEK_SET);

	// check file size
	if (*filesize == 0)
	{
		printf("Error: input HLSL shader file has invalid size!\n");
		return nullptr;
	}

	auto code = static_cast<char*>(malloc(*filesize));
	memset(code, 0, *filesize);
	fread(code, *filesize, 1, file);

	code[*filesize - 1] = '\0';

	return code;
}

void get_filenamepath(const char* filename, char* buffer, int buffersize, bool replace_slashes = true)
{
	memset(buffer, 0, buffersize);

	auto len = strlen(filename);

	auto found = false;
	auto endindex = 0;
	for (auto i = len - 1; i > 0; i--)
	{
		auto character = filename[i];

		if (character == '/' || character == '\\')
		{
			found = true;
			endindex = i;
			break;
		}
	}

	if (!found)
		return;

	// build path
	for (auto i = 0; i <= endindex; i++)
	{
		if (replace_slashes && filename[i] == '\\')
		{
			buffer[i] = '/';
			continue;
		}

		buffer[i] = filename[i];
	}
}

bool compile_shadermethod(char* code, int codelength, const char* input, const char* method, const char* profile, int shadertype, File& file)
{
	static char full_profile[6] = {};
	memset(full_profile, 0, 6);

	switch (shadertype)
	{
	case VERTEX_SHADER:
		strcat(full_profile, "vs_");
		break;
	case PIXEL_SHADER:
		strcat(full_profile, "ps_");
		break;
	case COMPUTE_SHADER:
		strcat(full_profile, "cs_");
		break;

	default:
		printf("Error: Invalid shader type.\n");
		return false;
	}
	strcat(full_profile, profile);

	// shader type
	file.write(shadertype);

	// TODO: write only selected targets
	// TODO: use defines

	ID3D10Blob* shaderBlob;
	ID3D10Blob* errorBlob;
	auto hr = D3DCompile(code, codelength, input, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, method, full_profile, 0, 0, &shaderBlob, &errorBlob);

	if (FAILED(hr))
	{
		printf(static_cast<char*>(errorBlob->GetBufferPointer()));
		return false;
	}

	// write d3d
	auto d3dlen = static_cast<int>(shaderBlob->GetBufferSize());
	file.write(d3dlen);
	file.write(shaderBlob->GetBufferPointer(), d3dlen);

	// translate to GLSL
	GLSLShader shader = {};
	HLSLccSamplerPrecisionInfo prec = {};
	HLSLccReflection reflection = {};
	GLSLCrossDependencyData deps = {};
	GlExtensions ext = {};

	TranslateHLSLFromMem(static_cast<char*>(shaderBlob->GetBufferPointer()), 0, LANG_400, &ext, &deps, prec, reflection, &shader);

	// write glsl
	auto length = static_cast<int>(shader.sourceCode.length());
	file.write(length);
	file.write((void*)shader.sourceCode.data(), length);

	return true;
}

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
bool Compiler::compile(const char* input, const char* output, const char* profile, bool dx, bool gl, bool vlk)
{
	// File structure 
	// ShaderFile [
	//
	//  ShaderMeta,
	//
	//  int PassCount,
	//  {
	//
	//    int ShaderCount,
	//    {
	//
	//      int ShaderType,
	//
	//      int d3dLength,
	//      byte* d3d,
	//
	//      int glslLength,
	//      byte* glsl
	//    }
	//
	//  }
	//
	// ]

	printf("\n"); // new line for a good start :>

	// get base file path
	char basepath[260] = {};
	get_filenamepath(input, basepath, 260);

	// the read shader file
	int codelength;
	auto code = readfile(input, &codelength);

	if (!code)
		return false;

	// preprocess
	shadermeta meta = {};
	if (!Parser::parse(code, codelength, &meta))
		return false;

	if (meta.passes.size() == 0u)
	{
		printf("Error: Shader has no any pass!");
		return false;
	}

	File file = {};
	File::openFile(&file, output, OpenMode::OpenWrite);
	file.seek(0);
	
	// write the meta into the file
	meta.write(file);
	
	file.write(static_cast<int>(meta.passes.size()));

	for (auto i = 0u; i < meta.passes.size(); i++)
	{
		auto pass = meta.passes[i];

		auto shadercount = 0;

		if (pass.vs_method[0] != '\0')
			shadercount++;
		if (pass.ps_method[0] != '\0')
			shadercount++;
		if (pass.cs_method[0] != '\0')
			shadercount++;

		file.write(shadercount);

		if (pass.vs_method[0] != '\0')
		{
			if (!compile_shadermethod(code, codelength, input, pass.vs_method.c_str(), pass.vs_profile.c_str(), VERTEX_SHADER, file))
				return false;
		}

		if (pass.ps_method[0] != '\0')
		{
			if (!compile_shadermethod(code, codelength, input, pass.ps_method.c_str(), pass.ps_profile.c_str(), PIXEL_SHADER, file))
				return false;
		}

		if (pass.cs_method[0] != '\0')
		{
			if (!compile_shadermethod(code, codelength, input, pass.cs_method.c_str(), pass.cs_profile.c_str(), COMPUTE_SHADER, file))
				return false;
		}
	}

	// done
	file.flush();
	file.close();

	// free the code
	free(code);

	return false;
}
