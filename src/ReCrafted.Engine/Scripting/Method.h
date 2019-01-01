// ReCrafted (c) 2016-2019 Always Too Late

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
    void Invoke() const;
    void Invoke(void** params) const;
    void InvokeStatic() const;
    void InvokeStatic(void** params) const;
};

#endif // METHOD_H
