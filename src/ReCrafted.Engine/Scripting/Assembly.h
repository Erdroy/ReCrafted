// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ASSEMBLY_H
#define ASSEMBLY_H

// includes
#include "Core/Types.h"
#include "Class.h"
#include "Mono.h"

class Assembly
{
    friend class ScriptingEngine;
    friend class Domain;
    friend class Class;
    friend class Object;

private:
    MonoDomain* m_domain = nullptr;

    MonoAssembly* m_assembly = nullptr;
    MonoImage* m_image = nullptr;

public:
    /// <summary>
    /// Finds class using namespace and class name.
    /// </summary>
    Ref<Class> FindClass(const char* class_namespace, const char* class_name);

public:
    static Ref<Assembly> API;
    static Ref<Assembly> Game;
};

#endif // ASSEMBLY_H
