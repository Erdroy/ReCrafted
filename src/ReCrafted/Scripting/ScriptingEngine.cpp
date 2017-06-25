// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "ScriptingEngine.h"
#include "../Core/Logger.h"

#include <mono/metadata/debug-helpers.h>

#pragma comment(lib, "mono.lib")

MonoDomain* m_domain;

MonoAssembly* m_api_assembly;
MonoAssembly* m_core_assembly;

MonoObject* m_game_main;

MonoMethod* m_method_initialize;
MonoMethod* m_method_update;
MonoMethod* m_method_simulate;
MonoMethod* m_method_shutdown;

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

	m_api_assembly = mono_domain_assembly_open(m_domain, "ReCrafted.API.dll");

	Logger::write("Loaded ReCraftedAPI.dll", LogLevel::Info);

	m_core_assembly = mono_domain_assembly_open(m_domain, "ReCraftedCore.dll");

	Logger::write("Loaded ReCraftedCore.dll", LogLevel::Info);

	// bind API
	bind_all();

	// create and run GameMain
	auto image = mono_assembly_get_image(m_core_assembly);
	auto m_game_main_obj = mono_class_from_name(image, "ReCraftedCore", "GameMain");
	m_game_main = mono_object_new(m_domain, m_game_main_obj);
	mono_runtime_object_init(m_game_main);

	auto initmethoddesc = mono_method_desc_new("ReCraftedCore.GameMain::Initialize", true);
	m_method_initialize = mono_method_desc_search_in_class(initmethoddesc, m_game_main_obj);
	
	initmethoddesc = mono_method_desc_new("ReCraftedCore.GameMain::Update", true);
	m_method_update = mono_method_desc_search_in_class(initmethoddesc, m_game_main_obj);

	initmethoddesc = mono_method_desc_new("ReCraftedCore.GameMain::Simulate", true);
	m_method_simulate = mono_method_desc_search_in_class(initmethoddesc, m_game_main_obj);

	initmethoddesc = mono_method_desc_new("ReCraftedCore.GameMain::Shutdown", true);
	m_method_shutdown = mono_method_desc_search_in_class(initmethoddesc, m_game_main_obj);

	initialize();
}

void ScriptingEngine::load_assembly(const char* assemblyName)
{

	Logger::write("Loade assembly '", assemblyName, "'", LogLevel::Info);
}

void ScriptingEngine::initialize()
{
	mono_runtime_invoke(m_method_initialize, m_game_main, nullptr, nullptr);
}

void ScriptingEngine::update()
{
	mono_runtime_invoke(m_method_update, m_game_main, nullptr, nullptr);
}

void ScriptingEngine::simulate()
{
	mono_runtime_invoke(m_method_simulate, m_game_main, nullptr, nullptr);
}

void ScriptingEngine::shutdown()
{
	mono_runtime_invoke(m_method_shutdown, m_game_main, nullptr, nullptr);

	Logger::write("Shutting down ScriptingEngine...", LogLevel::Info);

	mono_jit_cleanup(m_domain);
}
