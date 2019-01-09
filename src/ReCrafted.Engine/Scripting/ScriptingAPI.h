// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef SCRIPTINGAPI_H
#define SCRIPTINGAPI_H

#include "Core/Action.h"
#include "Common/ReCraftedAPI.h"
#include "Scripting/Bindings.h"
#include "Scripting/Mono.h"
#include "Scripting/Script.h"

#define MAIN_THREAD_ONLY() \
    _ASSERT_(IS_MAIN_THREAD(), __FUNCTION__" can be called only from main thread!");

#define mono_object_get_type(obj) \
    static_cast<MonoTypeEnum>(mono_type_get_type(mono_class_get_type(mono_object_get_class(obj))));

#endif // SCRIPTINGAPI_H
