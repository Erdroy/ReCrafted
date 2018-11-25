// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RIGIDBODYACTOR_H
#define RIGIDBODYACTOR_H

#include "ReCrafted.h"
#include "Common/Actors/Actor.h"
#include "Physics/Collision.h"
#include "Physics/IPhysicsActor.h"

/**
 * \brief RigidBody physics actor.
 * 
 * \note To create actor of this type use RigidBodyActor::Create* functions.
 */
class RigidBodyActor final : public Actor<RigidBodyActor>
{
private:
    SCRIPTING_API_IMPL()

private:
    IPhysicsActor* m_actor = nullptr;
    Collision* m_collision = nullptr;
    bool m_dynamic = true;

    Vector3 m_gravity = Vector3::Zero;

private:
    void Initialize();

    void OnStart() override;
    void OnUpdate() override;
    void OnSimulate() override;
    void OnDestroy() override;

    const char* GetObjectName() const override
    {
        return "RigidBodyActor";
    }

public:
    void AttachCollision(Collision* collision);
    void DetachCollision();

public:
    void SetPosition(const Vector3& position) override;
    void SetRotation(const Quaternion& rotation) override;

    void AddForce(const Vector3& force, ForceMode forceMode, bool awake = true) const;
    void AddTorque(const Vector3& torque, ForceMode forceMode, bool awake = true) const;

    void SetCollisionLayer(uint32_t layer) const;

    void SetVelocity(const Vector3& velocity) const;
    Vector3 GetVelocity() const;

    void SetAngularVelocity(const Vector3& angularVelocity) const;
    Vector3 GetAngularVelocity() const;

    void SetCentreOfMass(const Vector3& massCentre) const;
    Vector3 GetCentreOfMass() const;

    void SetMaxAngularVelocity(float maxAngularVelocity) const;
    float GetMaxAngularVelocity() const;

    void SetLinearDamping(float damping) const;
    float GetLinearDamping() const;

    void SetAngularDamping(float angularDamping) const;
    float GetAngularDamping() const;

    void SetMass(float mass) const;
    float GetMass() const;

    bool IsDynamic() const;

    void IsSleeping(bool sleep) const;
    bool IsSleeping() const;

    void SetCCD(bool enabled) const;
    bool GetCCD() const;

    void IsKinematic(bool isKinematic) const;
    bool IsKinematic() const;
public:
    /**
     * \brief Creates and initializes new dynamic actor.
     * \return The newly created dynamic actor.
     */
    static RigidBodyActor* Create();

    /**
     * \brief Creates and initializes new dynamic actor.
     * \return The newly created dynamic actor.
     */
    static RigidBodyActor* CreateDynamic();

    /**
     * \brief Creates and initializes new static actor.
     * \return The newly created static actor.
     */
    static RigidBodyActor* CreateStatic();
};

#endif // RIGIDBODYACTOR_H