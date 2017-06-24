// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "ScriptingEngine.h"
#include "../Core/Logger.h"

#pragma comment(lib, "mono.lib")

MonoDomain* m_domain;

MonoAssembly* m_api_assembly;
MonoAssembly* m_core_assembly;

void ScriptingEngine::run()
{
	Logger::write("Initializing Mono ScriptingEngine", LogLevel::Info);

	if (m_domain != nullptr)
	{
		Logger::write("Tried to run the ScriptingEngine second time!", LogLevel::Warning);
		return;
	}

	mono_set_dirs("../mono/lib", "../mono/etc");

	m_domain = mono_jit_init_version("ReCrafted", "v4.0.30319");

	//m_api_assembly = mono_domain_assembly_open(m_domain, "ReCraftedAPI.dll");

	//Logger::write("Loaded ReCraftedAPI.dll", LogLevel::Info);

	m_core_assembly = mono_domain_assembly_open(m_domain, "ReCraftedCore.dll");

	Logger::write("Loaded ReCraftedCore.dll", LogLevel::Info);

	// bind API
	bind_all();
}

void ScriptingEngine::load_assembly(const char* assemblyName)
{

	Logger::write("Loade assembly '", assemblyName, "'", LogLevel::Info);
}

void ScriptingEngine::shutdown()
{
	Logger::write("Shutting down ScriptingEngine...", LogLevel::Info);

	mono_jit_cleanup(m_domain);
}
