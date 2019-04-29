// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Domain.h"
#include "Assembly.h"

List<RefPtr<Domain>> Domain::Domains;
RefPtr<Domain> Domain::Root;

RefPtr<Assembly> Domain::LoadAssembly(const char* fileName)
{
    // Open assembly file and load it into the memory
    const auto monoAssembly = mono_domain_assembly_open(m_domain, fileName);
    ASSERT(monoAssembly);

    // Get assembly image
    const auto monoImage = mono_assembly_get_image(monoAssembly);
    ASSERT(monoImage);

    // Create assembly
    const RefPtr<Assembly> assembly(new Assembly(monoAssembly, monoImage, m_domain));

    // Add to the loaded assembly list
    m_assemblies.Add(assembly);

    return assembly;
}

void Domain::Finalize(const uint32_t timeout) const
{
    mono_domain_finalize(m_domain, timeout);
}

void Domain::Cleanup()
{
    mono_jit_cleanup(m_domain);
    m_domain = nullptr;
}

RefPtr<Domain> Domain::CreateRoot(MonoDomain* monoDomain)
{
    const RefPtr<Domain> domain(new Domain(monoDomain));
    Domains.Add(domain);
    Root = domain;
    return domain;
}

RefPtr<Domain> Domain::Create(const char* name)
{
    const auto monoDomain = mono_domain_create_appdomain(const_cast<char*>(name), nullptr);
    const RefPtr<Domain> domain(new Domain(monoDomain));
    Domains.Add(domain);
    return domain;
}
