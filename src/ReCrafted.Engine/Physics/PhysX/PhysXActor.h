// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSXACTOR_H
#define PHYSXACTOR_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Math.h"
#include "Physics/IPhysicsActor.h"

#include "PhysX.h"

class PhysXActor : public IPhysicsActor
{
public:
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

    bool IsDynamic() override
    {
        return m_dynamic;
    }
};

#endif // PHYSXACTOR_H
