// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IPHYSICSACTOR_H
#define IPHYSICSACTOR_H

class IPhysicsShape;
enum class ForceMode;

class IPhysicsActor
{
public:
    virtual ~IPhysicsActor() = default;

public:
    virtual void AttachShape(IPhysicsShape* shape) = 0;
    virtual void DetachShape(IPhysicsShape* shape) = 0;

    virtual void SetPosition(const Vector3& position) = 0;
    virtual void SetRotation(const Quaternion& rotation) = 0;

    virtual void AddForce(const Vector3& force, ForceMode forceMode, bool awake = true) = 0;
    virtual void AddTorque(const Vector3& torque, ForceMode forceMode, bool awake = true) = 0;

    virtual Vector3 GetPosition() = 0;
    virtual Quaternion GetRotation() = 0;

    virtual void SetVelocity(const Vector3& velocity) = 0;
    virtual Vector3 GetVelocity() = 0;

    virtual void SetAngularVelocity(const Vector3& angularVelocity) = 0;
    virtual Vector3 GetAngularVelocity() = 0;

    virtual void SetCentreOfMass(const Vector3& massCentre) = 0;
    virtual Vector3 GetCentreOfMass() = 0;

    virtual void SetMaxAngularVelocity(float maxAngularVelocity) = 0;
    virtual float GetMaxAngularVelocity() = 0;

    virtual void SetLinearDamping(float damping) = 0;
    virtual float GetLinearDamping() = 0;

    virtual void SetAngularDamping(float angularDamping) = 0;
    virtual float GetAngularDamping() = 0;

    virtual void SetMass(float mass) = 0;
    virtual float GetMass() = 0;

    virtual bool IsDynamic() = 0;

    virtual void IsSleeping(bool sleep) = 0;
    virtual bool IsSleeping() = 0;

    virtual void SetCCD(bool enabled) = 0;
    virtual bool GetCCD() = 0;

    virtual void IsKinematic(bool isKinematic) = 0;
    virtual bool IsKinematic() = 0;

public:
    virtual void SetCollisionLayer(uint32_t layer) = 0;
    virtual uint32_t GetCollisionLayer() = 0;
};

#endif // IPHYSICSACTOR_H
