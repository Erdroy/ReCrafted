// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENTITYCOMPONENTSYSTEM_H
#define ENTITYCOMPONENTSYSTEM_H

// Base headers
#include "Common/Entities/System.h"
#include "Common/Entities/World.h"
#include "Common/Entities/Entity.h"

// Default components
#include "Common/TransformComponent.h"
#include "Scripting/ScriptingComponent.h"
#include "Audio/AudioSoundComponent.h"

#include <cassert>

class ECS
{
public:
    ECS() {
        // NOTE: Here you should add all of the native components,
        // this helps to make sure, that .NET and Native components 
        // have the same ComponentId.

        // NOTE: None of the engine components aren't initialized, so we cannot
        // use our ASSERT etc.

        /* 00 */ assert(GetComponentId<TransformComponent>() == 0);
        /* 01 */ assert(GetComponentId<ScriptingComponent>() == 1);
        /* 02 */ assert(GetComponentId<AudioSoundComponent>() == 2);
    }
};

#endif // ENTITYCOMPONENTSYSTEM_H
