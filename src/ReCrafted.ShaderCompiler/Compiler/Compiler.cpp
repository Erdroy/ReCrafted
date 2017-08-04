// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#define _CRT_SECURE_NO_WARNINGS

#include "Compiler.h"

#include <hlslcc.h>
#include <d3dcompiler.h>
#include <dxgiformat.h>
#include "ShaderMeta.h"
#include "Parser.h"

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

bool compile_shadermethod(char* code, int codelength, const char* input, char* method, char* profile, int shadertype, shadermeta_pass* pass, FILE* file)
{
	char full_profile[6] = {};
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

	ID3D10Blob* shaderBlob;
	ID3D10Blob* errorBlob;
	auto hr = D3DCompile(code, codelength, input, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, method, full_profile, 0, 0, &shaderBlob, &errorBlob);

	if (FAILED(hr))
	{
		printf(static_cast<char*>(errorBlob->GetBufferPointer()));
		return false;
	}

	// TODO: translate to only selected targets

	// translate to GLSL
	GLSLShader shader = {};
	HLSLccSamplerPrecisionInfo prec = {};
	HLSLccReflection reflection = {};
	GLSLCrossDependencyData deps = {};
	GlExtensions ext = {};

	TranslateHLSLFromMem(static_cast<char*>(shaderBlob->GetBufferPointer()), 0, LANG_400, &ext, &deps, prec, reflection, &shader);

	return false;
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

	if (meta.passes_count == 0u)
	{
		printf("Error: Shader has no any pass!");
		return false;
	}

	FILE* file = nullptr;
	fopen_s(&file, output, "w+");

	if (file == nullptr)
	{
		printf("Error: Failed to create output file '%s'.", output);
		return false;
	}

	fseek(file, 0, SEEK_SET);

	fwrite(&meta, sizeof shadermeta, 1u, file);

	// TODO: compile all passes
	// TODO: write all compiled bytecode

	fflush(file);

	// done!
	fclose(file);

	// free the code
	free(code);

	return false;
}
