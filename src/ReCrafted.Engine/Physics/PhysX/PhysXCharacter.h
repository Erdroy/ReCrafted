// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSXCHARACTER_H
#define PHYSXCHARACTER_H

#include "ReCrafted.h"
#include "Physics/IPhysicsCharacter.h"

#include "PhysX.h"

class PhysXCharacter : public IPhysicsCharacter
{
private:
    PxCapsuleController* m_controller;

public:
    explicit PhysXCharacter(PxCapsuleController* controller) : m_controller(controller) {}
    
public:
    CollisionFlags Move(const Vector3& vector) override;

    Vector3 GetVelocity() override;

    void SetPosition(const Vector3& position) override;
    Vector3 GetPosition() override;

    void SetRotation(const Quaternion& rotation) override;
    Quaternion GetRotation() override;
};

#endif // PHYSXCHARACTER_H
