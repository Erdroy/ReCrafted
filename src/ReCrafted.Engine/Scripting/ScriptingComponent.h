// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SCRIPTINGCOMPONENT_H
#define SCRIPTINGCOMPONENT_H

#include "Common/EntityComponentSystem.h"
#include "Core/Containers/Array.h"

struct ScriptingComponent : Component
{
public:
    Array<RefPtr<Script>> scripts = {};
};

#endif // SCRIPTINGCOMPONENT_H
