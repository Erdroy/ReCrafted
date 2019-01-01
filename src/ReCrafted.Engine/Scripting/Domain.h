// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef DOMAIN_H
#define DOMAIN_H

// includes
#include "Core/Types.h"
#include "Assembly.h"
#include "Mono.h"
#include "Core/Containers/Array.h"

#include <vector>

class Domain
{
    friend class ScriptingEngine;
    friend class Class;

private:
    MonoDomain* m_domain = nullptr;

    Array<RefPtr<Assembly>> m_loadedAssemblies = {};

public:
    /// <summary>
    /// Loads assembly using given fileName.
    /// </summary>
    RefPtr<Assembly> LoadAssembly(const char* fileName);

    /// <summary>
    /// Cleanups the domain.
    /// </summary>
    void Cleanup();

    MonoDomain* GetMono() const;

public:
    /// <summary>
    /// Creates Domain from MonoDomain.
    /// </summary>
    static RefPtr<Domain> Create(MonoDomain* monoDomain);

    /// <summary>
    /// Creates domain of given name.
    /// </summary>
    static RefPtr<Domain> Create(const char* name, RefPtr<Domain> parent);

    /// <summary>
    /// Creates root domain.
    /// </summary>
    static RefPtr<Domain> CreateRoot();

public:
    static RefPtr<Domain> Root;
};

#endif // DOMAIN_H
