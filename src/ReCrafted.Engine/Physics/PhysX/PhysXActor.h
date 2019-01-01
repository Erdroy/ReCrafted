// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef PHYSXACTOR_H
#define PHYSXACTOR_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Math.h"
#include "Physics/IPhysicsActor.h"

#include "PhysX.h"
#include "tbb/concurrent_vector.h"

class PhysXActor : public IPhysicsActor
{
public:
    uint32_t m_collisionLayer = 0u;

    PxRigidActor* actor;
    bool m_dynamic;
    
public:
    explicit PhysXActor(PxRigidActor* actor, const bool dynamic) : actor(actor), m_dynamic(dynamic) {}

public:
    void AttachShape(IPhysicsShape* shape) override;
    void DetachShape(IPhysicsShape* shape) override;

    void SetPosition(const Vector3& position) override;
    void SetRotation(const Quaternion& rotation) override;

    void AddForce(const Vector3& force, ForceMode forceMode, bool awake) override;
    void AddTorque(const Vector3& torque, ForceMode forceMode, bool awake) override;

    Vector3 GetPosition() override;
    Quaternion GetRotation() override;

    void SetVelocity(const Vector3& velocity) override;
    Vector3 GetVelocity() override;

    void SetAngularVelocity(const Vector3& angularVelocity) override;
    Vector3 GetAngularVelocity() override;

    void SetCentreOfMass(const Vector3& massCentre) override;
    Vector3 GetCentreOfMass() override;

    void SetMaxAngularVelocity(float maxAngularVelocity) override;
    float GetMaxAngularVelocity() override;

    void SetLinearDamping(float damping) override;
    float GetLinearDamping() override;

    void SetAngularDamping(float angularDamping) override;
    float GetAngularDamping() override;

    void SetMass(float mass) override;
    float GetMass() override;

    bool IsDynamic() override;

    void IsSleeping(bool sleep) override;
    bool IsSleeping() override;

    void SetCCD(bool enabled) override;
    bool GetCCD() override;

    void IsKinematic(bool isKinematic) override;
    bool IsKinematic() override;

public:
    void SetCollisionLayer(uint32_t layer) override;
    uint32_t GetCollisionLayer() override;
};

#endif // PHYSXACTOR_H
