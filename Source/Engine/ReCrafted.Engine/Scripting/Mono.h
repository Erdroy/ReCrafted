// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-debug.h>

#define mono_object_get_type(obj) \
    static_cast<MonoTypeEnum>(mono_type_get_type(mono_class_get_type(mono_object_get_class(obj))));
