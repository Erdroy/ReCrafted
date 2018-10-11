// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SCRIPTINGSYSTEM_H
#define SCRIPTINGSYSTEM_H

// includes
#include "ReCrafted.h"
#include "Common/EntityComponentSystem.h"

/**
 * \brief ScriptingSystem System. Provides hybridized ECS scripting.
 */
class ScriptingSystem : public System
{
public:
    ScriptingSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<ScriptingComponent>();
    }

public:
    void Initialize() override;
    void Shutdown() override;
    void Update() override;
    void Simulate();
};

#endif // SCRIPTINGSYSTEM_H
