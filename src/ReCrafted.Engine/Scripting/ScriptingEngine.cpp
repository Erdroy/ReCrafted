// ReCrafted (c) 2016-2018 Always Too Late

#include "ScriptingEngine.h"
#include "Bindings.h"
#include "Domain.h"
#include "Platform/Platform.h"

#include <mono/metadata/threads.h>

SINGLETON_IMPL(ScriptingEngine)

void ScriptingEngine::OnInit()
{
    // create root domain
    m_domain = Domain::CreateRoot();

    // check base files
    if (!Platform::FileExists("ReCrafted.Game.dll") || !Platform::FileExists("ReCrafted.API.dll"))
        exit(-1);

    // load base assemblies
    Assembly::Game = m_domain->LoadAssembly("ReCrafted.Game.dll");
    Assembly::API = m_domain->LoadAssembly("ReCrafted.API.dll");

    // apply bindings
    Bindings::Bind();
}

void ScriptingEngine::OnDispose()
{
    // Destroy all objects
    Object::DestroyAll();

    // Shutdown bindings
    Bindings::Shutdown();

    // Shutdown scripting engine
    m_domain->Cleanup();
}

void ScriptingEngine::Finalize()
{
    // Push finalizer
    mono_domain_finalize(m_instance->m_domain->GetMono(), 1000);
}

void ScriptingEngine::AttachCurrentThread()
{
    mono_thread_attach(m_instance->m_domain->GetMono());
}

void ScriptingEngine::DetachCurrentThread()
{
    mono_thread_detach(mono_thread_current());
}
