// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include "ClassTypeId.h"

class Component
{
public:
    virtual ~Component() = default;

};

template<typename T>
TypeId GetComponentId()
{
    static_assert(std::is_base_of<Component, T>(), "T must inherit from Component!");
    return ClassTypeId<Component>::GetTypeId<T>();
}

#endif // COMPONENT_H
