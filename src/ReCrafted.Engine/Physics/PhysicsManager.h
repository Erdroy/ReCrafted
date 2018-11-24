// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Physics/IPhysicsEngine.h"
#include "Scripting/ScriptingAPI.h"

enum class CollisionLayers : uint32_t
{
    None = 0,
    Default = 1 << 0,
    Character = 1 << 1,

    All = Default | Character
};

struct RayCastHit
{
public:
    Vector3 point;
    Vector3 normal;
    float distance;
    uint32_t faceIndex;
};

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

public: /* Scene Queries */
    static IPhysicsScene* GetSceneAt(Vector3 worldPosition);
    static bool RayCast(Vector3 position, Vector3 direction, float maxDistance, RayCastHit* hit, uint32_t collisionLayer = 0u);
};

#endif // PHYSICSMANAGER_H
