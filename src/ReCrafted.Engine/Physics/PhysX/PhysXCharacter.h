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

public:
    void SetUpDirection(const Vector3& upDirection) override;
    Vector3 GetUpDirection() override;

    void SetHeight(float height) override;
    float GetHeight() override;

    void SetRadius(float radius) override;
    float GetRadius() override;

    void SetStepOffset(float stepOffset) override;
    float GetStepOffset() override;

    void SetContactOffset(float contactOffset) override;
    float GetContactOffset() override;

    void SetSlopeLimit(float slopeLimit) override;
    float GetSlopeLimit() override;

    Vector3 GetVelocity() override;

    void SetPosition(const Vector3& position) override;
    Vector3 GetPosition() override;

    void SetRotation(const Quaternion& rotation) override;
    Quaternion GetRotation() override;

public:
    void SetCollisionLayer(uint32_t layer) override;
};

#endif // PHYSXCHARACTER_H
