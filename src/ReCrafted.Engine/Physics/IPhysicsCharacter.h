// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef IPHYSICSCHARACTER_H
#define IPHYSICSCHARACTER_H

#include "ReCrafted.h"

class IPhysicsCharacter
{
public:
    enum CollisionFlags
    {
        None = 0,
        Sides = 1 << 0,
        Up = 1 << 1,
        Down = 1 << 2
    };

public:
    virtual ~IPhysicsCharacter() = default;

public:
    virtual CollisionFlags Move(const Vector3& vector) = 0;

public:
    virtual void SetUpDirection(const Vector3& upDirection) = 0;
    virtual Vector3 GetUpDirection() = 0;

    virtual void SetHeight(float height) = 0;
    virtual float GetHeight() = 0;

    virtual void SetRadius(float radius) = 0;
    virtual float GetRadius() = 0;

    virtual void SetStepOffset(float stepOffset) = 0;
    virtual float GetStepOffset() = 0;

    virtual void SetContactOffset(float contactOffset) = 0;
    virtual float GetContactOffset() = 0;

    virtual void SetSlopeLimit(float slopeLimit) = 0;
    virtual float GetSlopeLimit() = 0;

    virtual Vector3 GetVelocity() = 0;

    virtual void SetPosition(const Vector3& position) = 0;
    virtual Vector3 GetPosition() = 0;

    virtual void SetRotation(const Quaternion& rotation) = 0;
    virtual Quaternion GetRotation() = 0;

public:
    virtual void SetCollisionLayer(uint32_t layer) = 0;
};

#endif // IPHYSICSCHARACTER_H
