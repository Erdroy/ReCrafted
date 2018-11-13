// ReCrafted (c) 2016-2018 Always Too Late

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

    virtual Vector3 GetVelocity() = 0;

    virtual void SetPosition(const Vector3& position) = 0;
    virtual Vector3 GetPosition() = 0;

    virtual void SetRotation(const Quaternion& rotation) = 0;
    virtual Quaternion GetRotation() = 0;
};

#endif // IPHYSICSCHARACTER_H
