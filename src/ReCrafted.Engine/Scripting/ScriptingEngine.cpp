// ReCrafted (c) 2016-2018 Always Too Late

#include "ScriptingEngine.h"
#include "Bindings.h"
#include "Domain.h"
#include "Platform/Platform.h"

SINGLETON_IMPL(ScriptingEngine)

void ScriptingEngine::onInit()
{
    // create root domain
    m_domain = Domain::createRoot();

    // check base files
    if (!Platform::fileExists("ReCrafted.Game.dll") || !Platform::fileExists("ReCrafted.API.dll"))
        exit(-1);

    // load base assemblies
    Assembly::Game = m_domain->loadAssembly("ReCrafted.Game.dll");
    Assembly::API = m_domain->loadAssembly("ReCrafted.API.dll");

    // apply bindings
    Bindings::bind();
}

void ScriptingEngine::onDispose()
{
    // destroy all objects
    Object::destroyall();

    // shutdown bindings
    Bindings::shutdown();

    // shutdown scripting engine
    m_domain->cleanup();
}
