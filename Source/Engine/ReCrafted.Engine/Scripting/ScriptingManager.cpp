// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ScriptingManager.h"
#include "Domain.h"
#include "Common/Platform/Environment.h"
#include "Assembly.h"
#include "Object.h"
#include "Method.h"
#include "Common/Logger.h"

const char* jit_options[] = {
    "--soft-breakpoints",
    "--debugger-agent=transport=dt_socket,address=127.0.0.1:55000,embedding=1,server=y,suspend=y,timeout=10000"
};

const char* rootDomainName = "ReCrafted";

void ScriptingManager::LoadAssemblies()
{
    m_apiAssembly = Domain::Root->LoadAssembly("./ReCrafted.API.dll");
    m_gameAssembly = Domain::Root->LoadAssembly("./ReCrafted.Game.dll");
    m_coreAssembly = Domain::Root->GetAssembly("mscorlib");
}

void ScriptingManager::Initialize()
{
    // Check if we need to attach debugger
    m_attachDebugger = Environment::GetCommandLineArguments().Contains(STRING_CONST("-debug"));

    mono_set_dirs("../Mono/lib", "../Mono/etc");

    if(m_attachDebugger)
    {
        mono_debug_init(MONO_DEBUG_FORMAT_MONO);
        mono_jit_parse_options(2, const_cast<char**>(jit_options));

        Logger::Log("Started mono debugger at address 127.0.0.1:55000 with settings:\n{0}\n{1}.", jit_options[0], jit_options[1]);
    }

    const auto monoDomain = mono_jit_init(rootDomainName);
    ASSERT(monoDomain);
    
    if (m_attachDebugger)
    {
        // Set domain for debugging debug
        mono_debug_domain_create(monoDomain);
    }

    // Create root domain
    Domain::CreateRoot(monoDomain);

    // Load assemblies
    LoadAssemblies();

    // Initialize runtime
    InitRuntime();
}

void ScriptingManager::Shutdown()
{
    // Finalize root domain
    Domain::Root->Finalize();

    // Unload assemblies
    Domain::Root->UnloadAssembly(m_gameAssembly);
    Domain::Root->UnloadAssembly(m_apiAssembly);

    // Cleanup and release
    Domain::Root->Cleanup();
}

void ScriptingManager::InternalThrowException(const ExceptionType type, const bool unhandled, const std::basic_string<char>& message)
{
    const char* _namespace;
    const char* _class;
    MonoImage* _image;

    switch (type) {
    case ExceptionType::MissingReferenceException:
        _image = GetAPIAssembly()->ToMonoImage();
        _namespace = "ReCrafted.API.Core";
        _class = "MissingReferenceException"; 
        break;
    case ExceptionType::ReCraftedException:
        _image = GetAPIAssembly()->ToMonoImage();
        _namespace = "ReCrafted.API.Core";
        _class = "ReCraftedException"; 
        break;
    case ExceptionType::Count:
    case ExceptionType::Exception:
    default:
        _image = GetCoreAssembly()->ToMonoImage();
        _namespace = "System";
        _class = "Exception";
        break;
    }

    const auto exception = mono_exception_from_name_msg(_image, _namespace, _class, message.c_str());

    if(unhandled)
    {
        mono_unhandled_exception(reinterpret_cast<MonoObject*>(exception));
    }
    else
    {
        mono_raise_exception(exception);
    }
}

const RefPtr<Assembly>& ScriptingManager::GetAPIAssembly()
{
    return GetInstance()->m_apiAssembly;
}

const RefPtr<Assembly>& ScriptingManager::GetGameAssembly()
{
    return GetInstance()->m_gameAssembly;
}

const RefPtr<Assembly>& ScriptingManager::GetCoreAssembly()
{
    return GetInstance()->m_coreAssembly;
}
