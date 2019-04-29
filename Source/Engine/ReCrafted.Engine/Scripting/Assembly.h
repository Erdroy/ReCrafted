// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Mono.h"

/// <summary>
///     Assembly class. Represents single assembly (loaded .dll file).
/// </summary>
class Assembly
{
private:
    MonoAssembly* m_assembly = nullptr;
    MonoImage* m_image = nullptr;
    MonoDomain* m_domain = nullptr;

public:
    Assembly(MonoAssembly* assembly, MonoImage* image, MonoDomain* domain) : m_assembly(assembly), m_image(image),
                                                                             m_domain(domain)
    { }

public:

};