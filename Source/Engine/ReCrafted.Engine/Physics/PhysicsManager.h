// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/SubSystems/SubSystem.h"
#include "Physics/PhysX.h"
#include "Physics/ShapeCooker.h"

enum class ForceMode
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
class PhysicsManager : public SubSystem<PhysicsManager>
{
    API_CLASS_BODY()

private:
    PxFoundation* m_foundation = nullptr;
    PxPvd* m_pvd = nullptr;
    PxPvdTransport* m_pvdTransport;

    PxPhysics* m_physics = nullptr;
    PxDefaultCpuDispatcher* m_cpuDispatcher = nullptr;

    PxMaterial* m_defaultMaterial = nullptr;

    PxTolerancesScale m_tolerancesScale;

    ShapeCooker* m_shapeCooker = nullptr;
    //std::vector<PhysXScene*> m_scenes = {};

protected:
    void Initialize() override;
    void Shutdown() override;

public:
    static PxPhysics* GetPhysics();

public: /* Scene Queries */
    /*static IPhysicsScene* GetSceneAt(Vector3 worldPosition);*/

    API_FUNCTION(noproxy)
    static bool RayCast(const Vector3& position, const Vector3& direction, float maxDistance, RayCastHit& hit, uint32_t collisionLayer);
};
