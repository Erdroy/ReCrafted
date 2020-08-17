// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Mono.h"

/// <summary>
///     Assembly class. Represents single assembly (loaded .dll file).
/// </summary>
class Assembly final
{
    friend class Domain;

private:
    MonoAssembly* m_assembly = nullptr;
    MonoImage* m_image = nullptr;
    MonoDomain* m_domain = nullptr;

private:
    void Unload();

public:
    /// <summary>
    ///     Initializes Assembly class instance.
    /// </summary>
    /// <param name="assembly">The mono assembly reference.</param>
    /// <param name="image">The mono image reference.</param>
    /// <param name="domain">The mono domain reference.</param>
    Assembly(MonoAssembly* assembly, MonoImage* image, MonoDomain* domain) : m_assembly(assembly), m_image(image),
                                                                             m_domain(domain)
    { }

public:
    /// <summary>
    ///     Gets class using namespace and class name.
    /// </summary>
    /// <param name="classNamespace">The class namespace</param>
    /// <param name="className">The class name.</param>
    /// <returns>The mono class wrapper structure.</returns>
    Class GetClass(const char* classNamespace, const char* className) const;

    /// <summary>
    ///     Returns Mono assembly pointer.
    /// </summary>
    /// <returns>The mono's assembly pointer.</returns>
    MonoAssembly* ToMono() const;

    /// <summary>
    ///     Returns Mono assembly image pointer.
    /// </summary>
    /// <returns>The mono's assembly image pointer.</returns>
    MonoImage* ToMonoImage() const;
};