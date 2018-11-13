// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXActor.h"
#include "PhysXShape.h"

void PhysXActor::AttachShape(IPhysicsShape* shape)
{
    ASSERT(shape);

    cvar physxShape = static_cast<PhysXShape*>(shape);
    actor->attachShape(*physxShape->shape);
}

void PhysXActor::DetachShape(IPhysicsShape* shape)
{
    ASSERT(shape);

    cvar physxShape = static_cast<PhysXShape*>(shape);
    actor->detachShape(*physxShape->shape);
}

void PhysXActor::SetPosition(const Vector3& position)
{
    cvar pose = actor->getGlobalPose();
    actor->setGlobalPose(PxTransform(position.x, position.y, position.z, pose.q));
}

void PhysXActor::SetRotation(const Quaternion& rotation)
{
    cvar pose = actor->getGlobalPose();
    actor->setGlobalPose(PxTransform(pose.p, PxQuat{ rotation.x, rotation.y, rotation.z, rotation.w }));
}

Vector3 PhysXActor::GetPosition()
{
    var pose = actor->getGlobalPose();
    rvar pos = pose.p;

    return {pos.x, pos.y, pos.z};
}

Quaternion PhysXActor::GetRotation()
{
    var pose = actor->getGlobalPose();
    rvar rot = pose.q;

    return { rot.x, rot.y, rot.z, rot.w };
}

void PhysXActor::SetVelocity(const Vector3& velocity)
{
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
}

Vector3 PhysXActor::GetVelocity()
{
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    cvar vel = dynamic->getLinearVelocity();
    return { vel.x, vel.y, vel.z };
}
