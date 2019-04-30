// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Scripting/Mono.h"

class Assembly;

/// <summary>
///     Domain class. Represents Mono scripting backend domain.
/// </summary>
class Domain
{
private:
    MonoDomain* m_domain = nullptr;
    RefPtr<Domain> m_self = nullptr;
    List<RefPtr<Assembly>> m_assemblies = {};

public:
    explicit Domain(MonoDomain* domain) : m_domain(domain) {}

public:
    /// <summary>
    ///     Loads assembly using given fileName.
    /// </summary>
    /// <remarks>
    ///     Once assembly is loaded, it cannot be unloaded.
    /// </remarks>
    RefPtr<Assembly> LoadAssembly(const char* fileName);

    /// <summary>
    ///     Releases reference to this assembly.
    /// </summary>
    /// <remarks>Assembly is released when all outstanding references are released.</remarks>
    /// <param name="assembly">The assembly reference pointer.</param>
    void UnloadAssembly(RefPtr<Assembly>& assembly);

    /// <summary>
    ///     Finalizes all finalize-pending objects in this domain.
    /// </summary>
    void Finalize(uint32_t timeout = -1) const;

    /// <summary>
    ///     Cleanups the domain.
    /// </summary>
    void Cleanup();

    /// <summary>
    ///     Returns Mono domain pointer.
    /// </summary>
    /// <returns>The mono's domain pointer.</returns>
    MonoDomain* ToMono() const
    {
        return m_domain;
    }

public:
    /// <summary>
    ///     Creates root Domain from MonoDomain.
    /// </summary>
    static RefPtr<Domain> CreateRoot(MonoDomain* monoDomain);

    /// <summary>
    ///     Creates new domain of given name with the root domain as parent.
    /// </summary>
    /// <param name="name">The domain name.</param>
    /// <remarks>Can be only used when the root domain is already initialized.</remarks>
    static RefPtr<Domain> Create(const char* name);

public:
    /// <summary>
    ///     Contains all domains.
    /// </summary>
    static List<RefPtr<Domain>> Domains;

    /// <summary>
    ///     The root Domain.
    /// </summary>
    static RefPtr<Domain> Root;
};