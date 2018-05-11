// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef CLASS_H
#define CLASS_H

// includes
#include "Mono.h"

class Class
{
    friend class ScriptingEngine;
    friend class Assembly;
    friend class Object;
    friend class Method;

private:
    Assembly* m_assembly = nullptr;
    MonoClass* m_class = nullptr;

public:
    /// <summary>
    /// Gets mono type.
    /// </summary>
    char* getType() const;
};

#endif // CLASS_H
