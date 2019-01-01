// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include "ClassTypeId.h"

struct Component { };

template<typename T>
TypeId GetComponentId()
{
    static_assert(std::is_base_of<Component, T>(), "T must inherit from Component!");
    return ClassTypeId<Component>::GetTypeId<T>();
}

#endif // COMPONENT_H
