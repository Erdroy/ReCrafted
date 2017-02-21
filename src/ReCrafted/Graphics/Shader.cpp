// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Shader.h"
#include "../Platform/Platform.h"

#include <json.hpp>

void Shader::init(const char* vs, const char* fs, const char* def)
{
	if(!Platform::fileExists(vs) || !Platform::fileExists(fs))
	{
		_ASSERT(false); // throw error
	}

	File vs_file = {};
	File fs_file = {};

	// open files
	Platform::openFile(&vs_file, vs, OpenMode::OpenRead);
	Platform::openFile(&fs_file, fs, OpenMode::OpenRead);

	auto vs_memory = bgfx::alloc(static_cast<uint>(vs_file.FileSize));
	auto fs_memory = bgfx::alloc(static_cast<uint>(fs_file.FileSize));

	vs_file.read(vs_memory->data);
	fs_file.read(fs_memory->data);

	m_vertexshader = bgfx::createShader(vs_memory);
	m_fragmentshader = bgfx::createShader(fs_memory);

	m_program = bgfx::createProgram(m_vertexshader, m_fragmentshader, true);

	vs_file.close();
	fs_file.close();

	if(Platform::fileExists(def))
	{
		// load shader definition
		File def_file = {};
		Platform::openFile(&def_file, def, OpenMode::OpenRead);
		
		// read data
		auto data = new char[def_file.FileSize];
		def_file.read(data, def_file.FileSize);

		// trim end
		for(auto i = def_file.FileSize-1; i >= 0; i --)
		{
			if (data[i] == '}')
				break;

			data[i] = '\0';
		}

		// read desc data
		auto deffilejson = nlohmann::json::parse(data);
		auto shadername = deffilejson["name"].get<std::string>();

		for(auto i = 0; i < deffilejson["uniforms"].size(); i ++)
		{
			
		}

		// close and release all resources
		delete[] data;
		def_file.close();
	}
	else
	{
		VS_LOG("WARNING: Loaded shader without description file!");
	}
}

void Shader::dispose()
{
	bgfx::destroyProgram(m_program);
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
	char defPath[512] = {};

	// build file name string
	strcat_s(vsPath, shaderPath);
	strcat_s(fsPath, shaderPath);
	strcat_s(defPath, shaderPath);

	strcat_s(vsPath, shaderName);
	strcat_s(fsPath, shaderName);
	strcat_s(defPath, shaderName);

	strcat_s(vsPath, "/vs_");
	strcat_s(fsPath, "/fs_");
	strcat_s(defPath, "/desc.json");

	strcat_s(vsPath, shaderName);
	strcat_s(fsPath, shaderName);

	strcat_s(vsPath, ".bin");
	strcat_s(fsPath, ".bin");

	// initialize shader class, and load shaders
	shader->init(vsPath, fsPath, defPath);

	return shader;
}
