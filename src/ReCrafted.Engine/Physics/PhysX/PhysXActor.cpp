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
}

void PhysXActor::SetRotation(const Quaternion& position)
{
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
