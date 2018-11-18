// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXCharacter.h"
#include "Common/Time.h"

extern PxPhysics* GPxPhysX;

IPhysicsCharacter::CollisionFlags PhysXCharacter::Move(const Vector3& vector)
{
    cvar moveDir = PxVec3(vector.x, vector.y, vector.z);

    // Create filters
    PxControllerFilters filters;
    filters.mFilterFlags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER;
    
    var flags = m_controller->move(moveDir, 0.001f, Time::FixedDeltaTime(), filters);
    return *reinterpret_cast<CollisionFlags*>(&flags);
}

void PhysXCharacter::SetUpDirection(const Vector3& upDirection)
{
    m_controller->setUpDirection({ upDirection.x, upDirection.y, upDirection.z });
}

Vector3 PhysXCharacter::GetUpDirection()
{
    cvar dir = m_controller->getUpDirection();
    return  { dir.x, dir.y, dir.z };
}

void PhysXCharacter::SetHeight(const float height)
{
    m_controller->resize(height);
}

float PhysXCharacter::GetHeight()
{
    return m_controller->getHeight();
}

void PhysXCharacter::SetRadius(const float radius)
{
    m_controller->setRadius(radius);
}

float PhysXCharacter::GetRadius()
{
    return m_controller->getRadius();
}

void PhysXCharacter::SetStepOffset(const float stepOffset)
{
    m_controller->setStepOffset(stepOffset);
}

float PhysXCharacter::GetStepOffset()
{
    return m_controller->getStepOffset();
}

void PhysXCharacter::SetContactOffset(const float contactOffset)
{
    m_controller->setContactOffset(contactOffset);
}

float PhysXCharacter::GetContactOffset()
{
    return m_controller->getContactOffset();
}

void PhysXCharacter::SetSlopeLimit(const float slopeLimit)
{
    m_controller->setSlopeLimit(Math::Cos(slopeLimit * Math::DegreeToRadian));
}

float PhysXCharacter::GetSlopeLimit()
{
    return Math::Acos(m_controller->getSlopeLimit()) * Math::RadianToDegree;
}

Vector3 PhysXCharacter::GetVelocity()
{
    cvar pos = m_controller->getActor()->getLinearVelocity();

    return { pos.x, pos.y, pos.z };
}

void PhysXCharacter::SetPosition(const Vector3& position)
{
    m_controller->setPosition({ position.x, position.y, position.z });
}

Vector3 PhysXCharacter::GetPosition()
{
    cvar pos = m_controller->getPosition();
    return { float(pos.x), float(pos.y), float(pos.z) };
}

void PhysXCharacter::SetRotation(const Quaternion& rotation)
{
    cvar actor = m_controller->getActor();

    cvar pose = actor->getGlobalPose();
    actor->setGlobalPose(PxTransform(pose.p, PxQuat{ rotation.x, rotation.y, rotation.z, rotation.w }));
}

Quaternion PhysXCharacter::GetRotation()
{
    cvar rotation = m_controller->getActor()->getGlobalPose().q;

    return { rotation.x, rotation.y, rotation.z, rotation.w };
}
