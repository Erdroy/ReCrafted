// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Physics/IPhysicsEngine.h"
#include "Scripting/ScriptingAPI.h"

class PhysicsManager : public EngineComponent<PhysicsManager>
{
    friend class EngineMain;
    friend class RigidBodyActor;

private:
    SCRIPTING_API_IMPL()

private:
    RefPtr<IPhysicsEngine> m_engine = nullptr;
    RefPtr<IPhysicsScene> m_defaultScene = nullptr;
    
private:
    void OnInit() override;
    void OnDispose() override;

    void Update() override;
    void Simulate();
    
public:
    static IPhysicsScene* Scene()
    {
        return m_instance->m_defaultScene.get();
    }

    static IPhysicsEngine* Engine()
    {
        return m_instance->m_engine.get();
    }
};

#endif // PHYSICSMANAGER_H
