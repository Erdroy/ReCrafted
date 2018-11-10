// ReCrafted (c) 2016-2018 Always Too Late

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

#endif // SCRIPTINGAPI_H
