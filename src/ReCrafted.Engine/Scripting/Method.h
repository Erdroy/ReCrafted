// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef METHOD_H
#define METHOD_H

// includes
#include "Mono.h"

class Method
{
    friend class Object;

private:
    MonoObject* m_object = nullptr;
    MonoMethod* m_method = nullptr;

public:
    void Invoke(void** params = nullptr) const;
    void InvokeStatic(void** params = nullptr) const;
};

#endif // METHOD_H
