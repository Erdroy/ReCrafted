// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/Actors/ActorBase.h"
#include "Physics/PhysX.h"

API_USING("ReCrafted.API.Core.Actors")
API_USING("ReCrafted.API.Mathematics")

/// <summary>
///     RigidBodyActor actor class.
/// </summary>
API_CLASS(public, abstract)
class RigidBodyActor : public ActorBase
{
    API_CLASS_BODY()
    friend class PhysicsManager;
    friend class PhysicsScene;

private:
    uint32_t m_collisionLayer = 0u;

    PxRigidActor* m_actor = nullptr;
    bool m_dynamic = true;
    bool m_interpolate = false;

    Vector3 m_gravity = Vector3::Zero;
    PhysicsScene* m_scene = nullptr;

public:
    ACTOR_BODY(RigidBodyActor)
    ~RigidBodyActor();

protected:
    virtual PxRigidActor* CreatePxActor(const PxTransform& transform) = 0;

protected:
    void OnAwake() override;
    void OnStart() override;

    void OnUpdate() override;
    void OnSimulate() override;

    void OnEnable() override;
    void OnDisable() override;

public:
    //void AttachShape();

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
    API_PROPERTY()
    void Velocity(const Vector3& velocity) const;

    /// <summary>
    ///     Gets or sets the velocity.
    /// </summary>
    API_PROPERTY()
    Vector3 Velocity() const;

    /// <summary>
    ///     Gets or sets the angular velocity.
    /// </summary>
    API_PROPERTY()
    void AngularVelocity(const Vector3& angularVelocity) const;

    /// <summary>
    ///     Gets or sets the angular velocity.
    /// </summary>
    API_PROPERTY()
    Vector3 AngularVelocity() const;

    /// <summary>
    ///     Gets or sets the centre of mass.
    /// </summary>
    API_PROPERTY()
    void CentreOfMass(const Vector3& massCentre) const;

    /// <summary>
    ///     Gets or sets the centre of mass.
    /// </summary>
    API_PROPERTY()
    Vector3 CentreOfMass() const;

    /// <summary>
    ///     Gets or sets the max of angular velocity.
    /// </summary>
    API_PROPERTY()
    void MaxAngularVelocity(float maxAngularVelocity) const;

    /// <summary>
    ///     Gets or sets the max of angular velocity.
    /// </summary>
    API_PROPERTY()
    float MaxAngularVelocity() const;

    /// <summary>
    ///     Gets or sets the linear damping.
    /// </summary>
    API_PROPERTY()
    void LinearDamping(float damping) const;

    /// <summary>
    ///     Gets or sets the linear damping.
    /// </summary>
    API_PROPERTY()
    float LinearDamping() const;

    /// <summary>
    ///     Gets or sets the angular damping.
    /// </summary>
    API_PROPERTY()
    void AngularDamping(float angularDamping) const;

    /// <summary>
    ///     Gets or sets the angular damping.
    /// </summary>
    API_PROPERTY()
    float AngularDamping() const;

    /// <summary>
    ///     Gets or sets the mass.
    /// </summary>
    API_PROPERTY()
    void Mass(float mass) const;

    /// <summary>
    ///     Gets or sets the mass.
    /// </summary>
    API_PROPERTY()
    float Mass() const;

    /// <summary>
    ///     Gets or sets the continuous collision detection active state.
    /// </summary>
    API_PROPERTY()
    void CCD(bool enabled) const;

    /// <summary>
    ///     Gets or sets the continuous collision detection active state.
    /// </summary>
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
    API_PROPERTY()
    void IsSleeping(bool sleep) const;

    /// <summary>
    ///     Gets the sleeping state. Returns true when this actor is sleeping or inactive.
    /// </summary>
    API_PROPERTY()
    bool IsSleeping() const;

    /// <summary>
    ///     Gets the kinematic state. Returns true when this actor is kinematic or inactive.
    /// </summary>
    API_PROPERTY()
    void IsKinematic(bool isKinematic) const;

    /// <summary>
    ///     Gets the kinematic state. Returns true when this actor is kinematic or inactive.
    /// </summary>
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