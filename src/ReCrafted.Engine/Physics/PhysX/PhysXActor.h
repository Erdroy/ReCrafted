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
    PxRigidActor* actor = nullptr;

public:
    explicit PhysXActor(PxRigidActor* actor) : actor(actor) {}

public:
    void AttachShape(IPhysicsShape* shape) override;
    void DetachShape(IPhysicsShape* shape) override;

    void SetPosition(const Vector3& position) override;
    void SetRotation(const Quaternion& position) override;

    Vector3 GetPosition() override;
    Quaternion GetRotation() override;

    void SetVelocity(const Vector3& velocity) override;
    Vector3 GetVelocity() override;
};

#endif // PHYSXACTOR_H
