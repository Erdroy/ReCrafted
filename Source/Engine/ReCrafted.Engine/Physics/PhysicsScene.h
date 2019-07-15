// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Physics/PhysX.h"
#include "Scripting/Object.h"

API_USING("ReCrafted.API.Mathematics");

/// <summary>
///     PhysicsScene class. Provides physics scene functionality and actor management.
///     After creation the scene is initially disabled, please set Enabled to true to allow simulation.
/// </summary>
API_CLASS(public, sealed, partial)
class PhysicsScene final : public Object
{
    API_CLASS_BODY()

private:
    PxScene* m_scene = nullptr;
    bool m_enabled = false;
    PxControllerManager* m_controllerManager = nullptr;

    PxMaterial* m_defaultMaterial = nullptr;
    uint8_t* m_scratchMemory = nullptr;

public:
    PhysicsScene();
    ~PhysicsScene();

public:
    void Update();
    void Simulate();

public:
    bool RayCast(const Vector3& position, const Vector3& direction, float maxDistance, RayCastHit* hit, uint32_t collisionLayer) const;
    
public:
    /// <summary>
    ///     Attaches given actor to this scene.
    /// </summary>
    void AttachActor(RigidBodyActor* actor);

    /// <summary>
    ///     Detaches given actor from the scene.
    /// </summary>
    void DetachActor(RigidBodyActor* actor);

public:
    /// <summary>
    ///     Gets or sets the scene's enable state. When true, the scene will be simulated.
    /// </summary>
    API_PROPERTY()
    void Enabled(const bool enabled)
    {
        m_enabled = enabled;
    }

    /// <summary>
    ///     Gets or sets the scene's enable state.
    /// </summary>
    API_PROPERTY()
    bool Enabled() const
    {
        return m_enabled;
    }
};
