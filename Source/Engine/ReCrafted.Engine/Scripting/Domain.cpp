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

void Domain::UnloadAssembly(RefPtr<Assembly>& assembly)
{
    ASSERT(assembly);
    ASSERT(m_assemblies.Contains(assembly));

    // Release assembly reference
    assembly->Unload();
    
    // Remove assembly from list
    m_assemblies.Remove(assembly);

    // Reset passed assembly reference pointer.
    assembly = nullptr;
}

void Domain::Finalize(const uint32_t timeout) const
{
    mono_domain_finalize(m_domain, timeout);
}

void Domain::Cleanup()
{
    mono_jit_cleanup(m_domain);
    m_domain = nullptr;

    // Remove from list
    Domains.Remove(m_self);

    // Reset root pointer if this domain is the root, too.
    if(Root == m_self)
        Root = nullptr;

    // Reset self reference
    m_self = nullptr;
}

RefPtr<Domain> Domain::CreateRoot(MonoDomain* monoDomain)
{
    const RefPtr<Domain> domain(new Domain(monoDomain));
    Domains.Add(domain);

    domain->m_self = domain;

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
