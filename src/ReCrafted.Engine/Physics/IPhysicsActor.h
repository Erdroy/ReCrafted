// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IPHYSICSACTOR_H
#define IPHYSICSACTOR_H

class IPhysicsShape;

class IPhysicsActor
{
public:
    virtual ~IPhysicsActor() = default;

public:
    virtual void AttachShape(IPhysicsShape* shape) = 0;
    virtual void DetachShape(IPhysicsShape* shape) = 0;

    virtual void SetPosition(const Vector3& position) = 0;
    virtual void SetRotation(const Quaternion& position) = 0;

    virtual Vector3 GetPosition() = 0;
    virtual Quaternion GetRotation() = 0;

    virtual void SetVelocity(const Vector3& velocity) = 0;
    virtual Vector3 GetVelocity() = 0;
};

#endif // IPHYSICSACTOR_H
