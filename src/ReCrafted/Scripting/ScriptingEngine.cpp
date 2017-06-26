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
	auto gamemain_class = load_class("ReCraftedCore", "GameMain");
	m_game_main = create_class_instance(gamemain_class);
	
	m_method_initialize = load_method("ReCraftedCore.GameMain::Initialize", gamemain_class);
	m_method_update = load_method("ReCraftedCore.GameMain::Update", gamemain_class);
	m_method_simulate = load_method("ReCraftedCore.GameMain::Simulate", gamemain_class);
	m_method_shutdown = load_method("ReCraftedCore.GameMain::Shutdown", gamemain_class);

	initialize();
}

MonoClass* ScriptingEngine::load_class(const char* classNamespace, const char* className)
{
	auto image = mono_assembly_get_image(m_core_assembly);
	return  mono_class_from_name(image, classNamespace, className);
}

MonoMethod* ScriptingEngine::load_method(const char* methodName, MonoClass* monoClass)
{
	auto initmethoddesc = mono_method_desc_new(methodName, true);
	return mono_method_desc_search_in_class(initmethoddesc, monoClass);
}

MonoObject* ScriptingEngine::create_class_instance(MonoClass* monoClass)
{
	auto instance = mono_object_new(m_domain, monoClass);
	mono_runtime_object_init(instance);

	return instance;
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
