// ReCrafted (c) 2016-2018 Always Too Late

#include "Domain.h"
#include "Core/Logger.h"
#include "Core/GameInfo.h"

Ref<Domain> Domain::Root;

const char* jit_options[] = {
    "--soft-breakpoints",
    "--debugger-agent=transport=dt_socket,address=127.0.0.1:55000"
};

const char* rootDomainName = "ReCrafted";
const char* runtimeVersion = "v4.0.30319";

Ref<Assembly> Domain::LoadAssembly(const char* fileName)
{
    // do mono related stuff
    auto masm = mono_domain_assembly_open(m_domain, fileName);
    auto mimg = mono_assembly_get_image(masm);

    if (!masm || !mimg)
        return nullptr;

    // create assembly instance
    Ref<Assembly> assembly(new Assembly);
    assembly->m_assembly = masm;
    assembly->m_image = mimg;
    assembly->m_domain = m_domain;

    // add to the loaded assembly list
    m_loadedAssemblies.Add(assembly);

    return assembly;
}

void Domain::Cleanup()
{
    //mono_jit_cleanup(m_domain);
    m_domain = nullptr;
}

MonoDomain* Domain::GetMono() const
{
    return m_domain;
}

Ref<Domain> Domain::Create(MonoDomain* monoDomain)
{
    Ref<Domain> domain(new Domain);
    domain->m_domain = monoDomain;
    return domain;
}

Ref<Domain> Domain::Create(const char* name, Ref<Domain> parent)
{
    Ref<Domain> domain(new Domain);

    domain->m_domain = mono_domain_create();

    return domain;
}

Ref<Domain> Domain::CreateRoot()
{
    if (Root)
    {
        Logger::LogWarning("Cannot create second root domain!");
        return nullptr;
    }

    mono_set_dirs("../mono/lib", "../mono/etc");

    if (GameInfo::ContainsArgument(TEXT("-debug")) && !GameInfo::ContainsArgument(TEXT("-nodebug")))
    {
        mono_jit_parse_options(2, const_cast<char**>(jit_options));
    }

    auto domain = mono_jit_init_version(rootDomainName, runtimeVersion);

    if (!domain)
    {
        Logger::LogException("Failed to create root domain! Domain Name: {0} Runtime Version: {1}", rootDomainName,
                             runtimeVersion);
        return nullptr;
    }

    if (!GameInfo::ContainsArgument(TEXT("-nodebug")))
    {
        mono_debug_init(MONO_DEBUG_FORMAT_MONO);
        mono_debug_domain_create(domain);
    }

    // create instance
    Root = Create(domain);
    return Root;
}
