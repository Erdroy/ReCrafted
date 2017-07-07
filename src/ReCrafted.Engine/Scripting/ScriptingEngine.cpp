// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "ScriptingEngine.h"
#include "../Core/Logger.h"

#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-debug.h>
#include "../Core/GameInfo.h"

#pragma comment(lib, "mono.lib")

MonoDomain* m_domain;

MonoAssembly* m_api_assembly;
MonoAssembly* m_core_assembly;

MonoObject* m_game_main;

MonoMethod* m_method_initialize;
MonoMethod* m_method_update;
MonoMethod* m_method_simulate;
MonoMethod* m_method_drawui;
MonoMethod* m_method_shutdown;

const char* jit_options[] = {
	"--soft-breakpoints",
	"--debugger-agent=transport=dt_socket,address=127.0.0.1:55000"
};

void ScriptingEngine::run()
{
	Logger::write("Initializing Mono ScriptingEngine", LogLevel::Info);

	if (m_domain != nullptr)
	{
		Logger::write("Tried to run the ScriptingEngine second time!", LogLevel::Warning);
		return;
	}

	mono_set_dirs("../mono/lib", "../mono/etc");

	if (GameInfo::containsArgument(TEXT("-debug")))
	{
		mono_jit_parse_options(2, const_cast<char**>(jit_options));
	}

	m_domain = mono_jit_init_version("ReCrafted", "v4.0.30319");

	mono_debug_init(MONO_DEBUG_FORMAT_MONO);
	mono_debug_domain_create(m_domain);

	m_api_assembly = mono_domain_assembly_open(m_domain, "ReCrafted.API.dll");
	
	Logger::write("Loaded ReCrafted.API.dll", LogLevel::Info);

	m_core_assembly = mono_domain_assembly_open(m_domain, "ReCrafted.Game.dll");

	Logger::write("Loaded ReCrafted.Game.dll", LogLevel::Info);

	// bind API
	bind_all();

	// create and run GameMain
	auto gamemain_class = load_class("ReCrafted.Game", "GameMain");
	m_game_main = create_class_instance(gamemain_class);
	
	m_method_initialize = load_method("ReCrafted.Game.GameMain::Initialize", gamemain_class);
	m_method_update = load_method("ReCrafted.Game.GameMain::Update", gamemain_class);
	m_method_simulate = load_method("ReCrafted.Game.GameMain::Simulate", gamemain_class);
	m_method_drawui = load_method("ReCrafted.Game.GameMain::DrawUI", gamemain_class);
	m_method_shutdown = load_method("ReCrafted.Game.GameMain::Shutdown", gamemain_class);
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

void ScriptingEngine::drawui()
{
	mono_runtime_invoke(m_method_drawui, m_game_main, nullptr, nullptr);
}

void ScriptingEngine::shutdown()
{
	mono_runtime_invoke(m_method_shutdown, m_game_main, nullptr, nullptr);

	Logger::write("Shutting down ScriptingEngine...", LogLevel::Info);

	mono_jit_cleanup(m_domain);
}
