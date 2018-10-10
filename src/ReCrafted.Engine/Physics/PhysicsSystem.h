// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

// includes
#include "ReCrafted.h"
#include "Common/Entities/System.h"
#include "IPhysicsEngine.h"

class PhysicsSystem : public System
{
private:
    static RefPtr<IPhysicsEngine> m_engine;

public:
    void Initialize() override;
    void Shutdown() override;

    void Update() override;
    void Simulate();

public:
    static IPhysicsEngine* Physics()
    {
        return m_engine.get();
    }
};

#endif // PHYSICSSYSTEM_H
