// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "ScriptingEngine.h"
#include "../Core/Logger.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#pragma comment(lib, "mono.lib")

MonoDomain* m_domain;

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
}

void ScriptingEngine::shutdown()
{
	Logger::write("Shutting down ScriptingEngine...", LogLevel::Info);

	mono_jit_cleanup(m_domain);
}
