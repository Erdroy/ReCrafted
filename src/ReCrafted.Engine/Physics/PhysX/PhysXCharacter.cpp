// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXCharacter.h"
#include "Common/Time.h"

extern PxPhysics* GPxPhysX;

IPhysicsCharacter::CollisionFlags PhysXCharacter::Move(const Vector3& vector)
{
    cvar moveDir = *reinterpret_cast<const PxVec3*>(&vector);

    // Create filters
    PxControllerFilters filters;
    filters.mFilterFlags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER;
    
    var flags = m_controller->move(moveDir, 0.01f, Time::FixedDeltaTime(), filters);
    return *reinterpret_cast<CollisionFlags*>(&flags);
}

Vector3 PhysXCharacter::GetVelocity()
{
    cvar pos = m_controller->getActor()->getLinearVelocity();

    return { pos.x, pos.y, pos.z };
}

void PhysXCharacter::SetPosition(const Vector3& position)
{
    cvar actor = m_controller->getActor();

    cvar pose = actor->getGlobalPose();
    actor->setGlobalPose(PxTransform(position.x, position.y, position.z, pose.q));
}

Vector3 PhysXCharacter::GetPosition()
{
    var pose = m_controller->getActor()->getGlobalPose();
    rvar pos = pose.p;

    return { pos.x, pos.y, pos.z };
}

void PhysXCharacter::SetRotation(const Quaternion& rotation)
{
    cvar actor = m_controller->getActor();

    cvar pose = actor->getGlobalPose();
    actor->setGlobalPose(PxTransform(pose.p, PxQuat{ rotation.x, rotation.y, rotation.z, rotation.w }));
}

Quaternion PhysXCharacter::GetRotation()
{
    var pose = m_controller->getActor()->getGlobalPose();
    rvar rot = pose.q;

    return { rot.x, rot.y, rot.z, rot.w };
}
