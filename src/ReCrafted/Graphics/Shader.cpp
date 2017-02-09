// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Shader.h"
#include "../Platform/Platform.h"

void Shader::init(const char* vs, const char* fs)
{
	if(!Platform::FileExists(vs) || !Platform::FileExists(fs))
	{
		_ASSERT(false); // throw error
	}

	File vs_file = {};
	File fs_file = {};

	// open files
	Platform::OpenFile(&vs_file, vs, OpenMode::OpenRead);
	Platform::OpenFile(&fs_file, fs, OpenMode::OpenRead);



	// read files
	// bgfx::createShader(
	// bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
}

Ptr<Shader> Shader::loadShader(const char* shaderName)
{
	Ptr<Shader> shader(new Shader);

	// select file base path
	const char* shaderPath = "???";
	switch (bgfx::getRendererType())
	{
	case bgfx::RendererType::Direct3D9:
		shaderPath = "../assets/shaders/dx9/";   
		break;

	case bgfx::RendererType::Direct3D11:
	case bgfx::RendererType::Direct3D12:
		shaderPath = "../assets/shaders/dx11/"; 
		break;

	case bgfx::RendererType::Metal:
		shaderPath = "../assets/shaders/metal/";
		break;

	case bgfx::RendererType::OpenGL:
		shaderPath = "../assets/shaders/glsl/"; 
		break;

	case bgfx::RendererType::Vulkan:
		shaderPath = "assets/shaders/spirv/"; 
		break;


	case bgfx::RendererType::Noop:
	case bgfx::RendererType::Gnm:
	case bgfx::RendererType::OpenGLES:
	case bgfx::RendererType::Count:
		_ASSERT(false);
	}

	char vsPath[512] = {};
	char fsPath[512] = {};

	// build file name string
	strcpy_s(vsPath, shaderPath);
	strcpy_s(fsPath, shaderPath);

	strcat_s(vsPath, "vs_");
	strcat_s(fsPath, "fs_");

	strcat_s(vsPath, shaderName);
	strcat_s(fsPath, shaderName);

	strcat_s(vsPath, ".bin");
	strcat_s(fsPath, ".bin");

	// initialize shader class, and load shaders
	//shader->init(vsPath, fsPath);

	return shader;
}
