// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef BINDINGS_H
#define BINDINGS_H

// includes
#include "Object.h"
#include "Core/Action.h"

class Bindings
{
public:
    static void Bind();
    static void Shutdown();

public:
    static void RegisterObject(const char* fullName, const Action<Object*>& createFunction);
    static Action<Object*>* GetObjectCreator(const char* fullName);
};

#endif // BINDINGS_H
