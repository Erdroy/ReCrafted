// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/Actors/ActorBase.h"
#include "Physics/PhysX.h"
#include "Physics/PhysicsScene.h"
#include "Physics/Colliders/Collider.h"

API_USING("ReCrafted.API.Core.Actors")
API_USING("ReCrafted.API.Mathematics")

enum class RigidBodySyncMode
{
    Default,
    Interpolation,
    Extrapolation,

    Count
};

/// <summary>
///     RigidBodyActor actor class.
/// </summary>
API_CLASS(public, abstract)
class RigidBodyActor : public ActorBase
{
    API_CLASS_BODY()
    friend class PhysicsManager;
    friend class PhysicsScene;

protected:
    uint32_t m_collisionLayer = 0u;

    PxRigidActor* m_actor = nullptr;
    bool m_dynamic = true;
    bool m_affectedByGravity = true;

    RigidBodySyncMode m_syncMode = RigidBodySyncMode::Default;

    Vector3 m_gravity = Vector3::Zero;
    PhysicsScene* m_scene = nullptr;

    PxVec3 m_currentPxPos = {};
    PxQuat m_currentPxRot = {};
    PxVec3 m_lastPxPos = {};
    PxQuat m_lastPxRot = {};

    std::vector<Collider*> m_colliders;

public:
    ~RigidBodyActor();

protected:
    void OnStart() override;

    void OnUpdate() override;
    void OnSimulate() override;

    void OnEnable() override;
    void OnDisable() override;

public:
    /// <summary>
    ///     Attaches given collider to this actor.
    /// </summary>
    /// <param name="collider">Collider to attach.</param>
    /// <param name="awake">
    ///     Awakes the actor after adding the collider to it.
    ///     Only available for dynamic RigidBodies.
    /// </param>
    API_FUNCTION()
    void AttachCollider(Collider* collider, bool awake = false);

    /// <summary>
    ///     Detaches given collider to this actor.
    /// </summary>
    /// <param name="collider">Collider to detach.</param>
    /// <param name="awakeOnLostTouch">
    ///     When this actors looses the touch with other collision objects, it will be awaken.
    /// </param>
    API_FUNCTION()
    void DetachCollider(Collider* collider, bool awakeOnLostTouch = true);

    /// <summary>
    ///     Detaches currently attached colliders from this actor.
    /// </summary>
    /// <param name="awakeOnLostTouch">
    ///     When this actors looses the touch with other collision objects, it will be awaken.
    /// </param>
    API_FUNCTION()
    void DetachColliders(bool awakeOnLostTouch = true);

    /// <summary>
    ///     Adds force to this actor.
    /// </summary>
    /// <param name="force">The torque.</param>
    /// <param name="forceMode">The mode of the force.</param>
    /// <param name="awake">When true, this actor will be awaken, if sleeping.</param>
    API_FUNCTION()
    void AddForce(const Vector3& force, ForceMode forceMode, bool awake = true) const;

    /// <summary>
    ///     Adds torque to this actor.
    /// </summary>
    /// <param name="torque">The torque.</param>
    /// <param name="forceMode">The mode of the force.</param>
    /// <param name="awake">When true, this actor will be awaken, if sleeping.</param>
    API_FUNCTION()
    void AddTorque(const Vector3& torque, ForceMode forceMode, bool awake = true) const;

public:
    /// <summary>
    ///     Gets the scene that this actor is attached to.
    /// </summary>
    API_PROPERTY()
    PhysicsScene* Scene() const;

    /// <summary>
    ///     Gets the currently attached colliders.
    /// </summary>
    API_PROPERTY(noprefix);
    Array<Collider*> GetColliders()
    {
        return { m_colliders.data(), m_colliders.size() };
    }

    /// <summary>
    ///     Gets or sets the sync mode of this actor.
    /// </summary>
    API_PROPERTY()
    void SyncMode(const RigidBodySyncMode& mode);

    /// <summary>
    ///     Gets or sets the sync mode of this actor.
    /// </summary>
    API_PROPERTY()
    RigidBodySyncMode SyncMode() const;

    /// <summary>
    ///     Gets or sets world-space position of this actor.
    /// </summary>
    API_PROPERTY()
    void Position(const Vector3& position) override;

    /// <summary>
    ///     Gets or sets world-space position of this actor.
    /// </summary>
    API_PROPERTY()
    const Vector3& Position() const override;

    /// <summary>
    ///     Gets or sets world-space position of this actor.
    /// </summary>
    API_PROPERTY()
    void Rotation(const Quaternion& rotation) override;

    /// <summary>
    ///     Gets or sets world-space position of this actor.
    /// </summary>
    API_PROPERTY()
    const Quaternion& Rotation() const override;

    /// <summary>
    ///     Gets or sets gravity affection state. When true, this actor will be affected by gravitation.
    /// </summary>
    API_PROPERTY()
    void AffectedByGravity(bool gravityEnabled);

    /// <summary>
    ///     Gets or sets gravity affection state. When true, this actor will be affected by gravitation.
    /// </summary>
    API_PROPERTY()
    bool AffectedByGravity() const;

    /// <summary>
    ///     Gets or sets the collision layer.
    /// </summary>
    API_PROPERTY()
    void CollisionLayer(uint32_t layer);

    /// <summary>
    ///     Gets or sets the collision layer.
    /// </summary>
    API_PROPERTY()
    uint32_t CollisionLayer() const;

    /// <summary>
    ///     Gets or sets the velocity.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    void Velocity(const Vector3& velocity) const;

    /// <summary>
    ///     Gets or sets the velocity.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    Vector3 Velocity() const;

    /// <summary>
    ///     Gets or sets the angular velocity.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    void AngularVelocity(const Vector3& angularVelocity) const;

    /// <summary>
    ///     Gets or sets the angular velocity.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    Vector3 AngularVelocity() const;

    /// <summary>
    ///     Gets or sets the centre of mass.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    void CentreOfMass(const Vector3& massCentre) const;

    /// <summary>
    ///     Gets or sets the centre of mass.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    Vector3 CentreOfMass() const;

    /// <summary>
    ///     Gets or sets the max of angular velocity.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    void MaxAngularVelocity(float maxAngularVelocity) const;

    /// <summary>
    ///     Gets or sets the max of angular velocity.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    float MaxAngularVelocity() const;

    /// <summary>
    ///     Gets or sets the linear damping.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    void LinearDamping(float damping) const;

    /// <summary>
    ///     Gets or sets the linear damping.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    float LinearDamping() const;

    /// <summary>
    ///     Gets or sets the angular damping.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    void AngularDamping(float angularDamping) const;

    /// <summary>
    ///     Gets or sets the angular damping.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    float AngularDamping() const;

    /// <summary>
    ///     Gets or sets the mass.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    void Mass(float mass) const;

    /// <summary>
    ///     Gets or sets the mass.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    float Mass() const;

    /// <summary>
    ///     Gets or sets the continuous collision detection active state.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    void CCD(bool enabled) const;

    /// <summary>
    ///     Gets or sets the continuous collision detection active state.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    bool CCD() const;

    /// <summary>
    ///     Gets the dynamic state. True when this actor is dynamic (i.e. derived from DynamicRigidBodyActor class).
    /// </summary>
    API_PROPERTY()
    bool IsDynamic() const;

    /// <summary>
    ///     Gets the sleeping state. Returns true when this actor is sleeping or inactive.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    void IsSleeping(bool sleep) const;

    /// <summary>
    ///     Gets the sleeping state. Returns true when this actor is sleeping or inactive.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    bool IsSleeping() const;

    /// <summary>
    ///     Gets the kinematic state. Returns true when this actor is kinematic or inactive.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    void IsKinematic(bool isKinematic) const;

    /// <summary>
    ///     Gets the kinematic state. Returns true when this actor is kinematic or inactive.
    /// </summary>
    /// <remarks>Can be used only on dynamic RigidBody.</remarks>
    API_PROPERTY()
    bool IsKinematic() const;

public:
    /// <summary>
    ///     Creates new dynamic RigidBodyActor.
    /// </summary>
    API_FUNCTION(multithread)
    static RigidBodyActor* Create();

    /// <summary>
    ///     Creates new dynamic RigidBodyActor.
    /// </summary>
    API_FUNCTION(multithread)
    static RigidBodyActor* CreateDynamic();

    /// <summary>
    ///     Creates new static RigidBodyActor.
    /// </summary>
    API_FUNCTION(multithread)
    static RigidBodyActor* CreateStatic();
};