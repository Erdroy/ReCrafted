// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ScriptingManager.h"

const char* jit_options[] = {
    "--soft-breakpoints",
    "--debugger-agent=transport=dt_socket,address=127.0.0.1:55000,embedding=1,server=y,suspend=n,timeout=10000"
};

const char* rootDomainName = "ReCrafted";

void ScriptingManager::Initialize()
{
    mono_set_dirs("../Mono/lib", "../Mono/etc");

    mono_debug_init(MONO_DEBUG_FORMAT_MONO);
    mono_jit_parse_options(2, const_cast<char**>(jit_options));

    m_rootDomain = mono_jit_init(rootDomainName);
    ASSERT(m_rootDomain);

    mono_debug_domain_create(m_rootDomain);
}

void ScriptingManager::Shutdown()
{
    const auto finalizeTimeout = 1000;

    mono_domain_finalize(m_rootDomain, finalizeTimeout);
    mono_jit_cleanup(m_rootDomain);
}
