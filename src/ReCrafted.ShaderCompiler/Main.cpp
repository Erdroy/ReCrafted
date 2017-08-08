// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

/*
* WARNING: Compile as 32bit application.
*/

#define _CRT_SECURE_NO_WARNINGS

#define MAX_PATH 260

#include <cstdio>
#include <cstring>

#include "Compiler/Compiler.h"
#include <string>

/*
* checks if string starts with other string, used for checking params.
* source: http://stackoverflow.com/questions/4770985/how-to-check-if-a-string-starts-with-another-string-in-c
*/
bool str_startsWith(const char *pre, const char *str)
{
	auto lenpre = strlen(pre), lenstr = strlen(str);
	return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

/*
* checks if string contains other string 'word'.
*/
bool str_contains(const char* word, const char* str)
{
	return strstr(str, word) != nullptr;
}

/*
* prints shader compiler usage
*/
void print_usage()
{
	printf("Usage:\n %s\n %s\n %s\n %s\n",
		"-i=\"\" - input file in hlsl, must be full path",
		"-o=\"\" - output file in hlsl, must be full path",
		"-p= - shader model profile (optional, 4_0 will be used)",
		"-t= - target rendering api's (optional, ALL will be used)"
	);
	printf("\nWarning: entry names can have only up to 16 characters!\n\n");

	printf("-------------------\n");
	printf("Targets: ALL(DX, GL, SPIRV), DX, GL, SPIRV\n");
	printf("Profiles: 3_0, 4_0 and 5_0 \n\n");
	printf("-------------------\n");
	printf("Example usage: \n");
	printf("Compile \n");
	printf("rcsc.exe \n\t-i=\"X:/shaders/test.hlsl\" \n\t-o=\"X:/shaders/test.shader\"  \n\t-p=4_0 \n\t-t=ALL\n\n");

	printf("Compile shader for dx and vulkan\n");
	printf("rcsc.exe \n\t-i=\"X:/shaders/test.hlsl\" \n\t-o=\"X:/shaders/test.shader\"  \n\t-p=5_0 \n\t-t=DX,SPIRV\n\n");

	printf("Compile shader for vulkan\n");
	printf("rcsc.exe \n\t-i=\"X:/shaders/test.hlsl\" \n\t-o=\"X:/shaders/test.shader\"  \n\t-p=5_0 \n\t-t=SPIRV\n\n");

	printf("Compile shader for vulkan with default settings\n");
	printf("rcsc.exe \n\t-i=\"X:/shaders/test.hlsl\" \n\t-o=\"X:/shaders/test.shader\"\n\n");
}

/*
 * Main entry point
 */
int main(int argc, char** argv)
{
	char inputFile[MAX_PATH] = {};
	char outputFile[MAX_PATH] = {};
	char profile[4] = {};
	char targets[16] = {};

	// check params count
	if (argc < 3) // file name, input and output
	{
		printf("\n\nInvalid arguments!\n\n");
		print_usage();
		return 1;
	}

	// read params
	for (auto i = 0; i < argc; i++)
	{
		auto arg = argv[i];

		if (str_startsWith("-i=", arg))
		{
			strcpy(inputFile, arg + 3);
		}
		else if (str_startsWith("-o=", arg))
		{
			strcpy(outputFile, arg + 3);
		}
		else if (str_startsWith("-p=", arg))
		{
			strcpy(profile, arg + 3);
		}
		else if (str_startsWith("-t=", arg))
		{
			strcpy(targets, arg + 3);
		}
	}

	if (inputFile[0] == '\0' || outputFile[0] == '\0')
	{
		printf("\n\nInvalid arguments!");
		print_usage();
		return 1;
	}

	if (profile[0] == '\0')
		strcpy(profile, "4_0");

	if (targets[0] == '\0')
		strcpy(targets, "ALL");

	auto all = str_contains("ALL", targets);
	auto dx = all || str_contains("DX", targets);
	auto gl = all || str_contains("GL", targets);
	auto vlk = all || str_contains("SPIRV", targets);

	printf("Starting compiling \"%s\" shader file for '%s' with profile '%s' on output path \"%s\"", inputFile, targets, profile, outputFile);

	if (Compiler::compile(inputFile, outputFile, profile, dx, gl, vlk))
	{
		printf("\n\nAll shaders compiled.");
		return 0;
	}

	printf("\n\nShader compilation failed!");
	return 1;
}