// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Lock.h"
#include "Core/SubSystems/SubSystem.h"
#include "Physics/PhysX.h"
#include "Physics/ShapeCooker.h"
#include "Physics/PhysicsScene.h"

enum class ForceMode : byte
{
    Force,
    Impulse,
    VelocityChange,
    Acceleration
};

struct RayCastHit
{
public:
    Vector3 Point;
    Vector3 Normal;
    float Distance;
    uint32_t FaceIndex;
};

API_USING("ReCrafted.API.Mathematics");

/// <summary>
///     PhysicsManager class. Provides basic physics functionality.
/// </summary>
API_CLASS(public, static, partial, noinherit)
class PhysicsManager final : public SubSystem<PhysicsManager>
{
    API_CLASS_BODY()
    friend class PhysicsScene;

private:
    PxFoundation* m_foundation = nullptr;
    PxPhysics* m_physics = nullptr;

    PxDefaultCpuDispatcher* m_cpuDispatcher = nullptr;

    PxPvd* m_pvd = nullptr;
    PxPvdTransport* m_pvdTransport = nullptr;

    ShapeCooker* m_defaultShapeCooker = nullptr;

    PxTolerancesScale m_tolerancesScale;

    Lock m_scenesLock;
    std::vector<PhysicsScene*> m_scenes = {};

private:
    void AddScene(PhysicsScene* scene);
    void RemoveScene(PhysicsScene* scene);

protected:
    void Initialize() override;
    void Shutdown() override;

protected:
    void OnUpdate() override;
    void OnFixedUpdate() override;

public:
    /// <summary>
    ///     Creates new scene for physics simulation.
    /// </summary>
    API_FUNCTION(multithread)
    static PhysicsScene* CreateScene();

    /// <summary>
    ///     Gets physics scene that contains given world position in it's bounds.
    ///     TODO: Add double-based math to API.
    /// </summary>
    API_FUNCTION(multithread)
    static PhysicsScene* GetSceneAt(Vector3 worldPosition);

public:
    API_FUNCTION(noproxy)
    static bool RayCast(const Vector3& position, const Vector3& direction, float maxDistance, RayCastHit& hit, uint32_t collisionLayer);

public:
    /// <summary>
    ///     Internal utility function for accessing PhysX's physics class instance.
    /// </summary>
    static PxPhysics* GetPhysics()
    {
        return GetInstance()->m_physics;
    }

    /// <summary>
    ///     Internal utility function for accessing PhysX's foundation class instance.
    /// </summary>
    static PxFoundation* GetFoundation()
    {
        return GetInstance()->m_foundation;
    }

    /// <summary>
    ///     Internal utility function for accessing PhysX's CPU dispatcher instance.
    /// </summary>
    static PxDefaultCpuDispatcher* GetCPUDispatcher()
    {
        return GetInstance()->m_cpuDispatcher;
    }

    /// <summary>
    ///     Internal utility function for accessing PhysX's tolerances scale.
    /// </summary>
    static const PxTolerancesScale& GetDefaultTolerances()
    {
        return GetInstance()->m_tolerancesScale;
    }

    /// <summary>
    ///     Internal utility function for accessing default shape cooker instance.
    /// </summary>
    static ShapeCooker* GetShapeCooker()
    {
        return GetInstance()->m_defaultShapeCooker;
    }
};
