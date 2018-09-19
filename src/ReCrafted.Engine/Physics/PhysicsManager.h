// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "IPhysicsEngine.h"

class PhysicsManager : public EngineComponent<PhysicsManager>
{
private:
    friend class EngineMain;

private:
    RefPtr<IPhysicsEngine> m_engine = nullptr;

public:
    virtual ~PhysicsManager() = default;

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;
    void Simulate();

public:
    static IPhysicsEngine* Physics()
    {
        return m_instance->m_engine.get();
    }
};

#endif // PHYSICSMANAGER_H
