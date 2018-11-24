// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXActor.h"
#include "PhysXShape.h"
#include "Physics/PhysicsManager.h"

void PhysXActor::AttachShape(IPhysicsShape* shape)
{
    ASSERT(shape);

    cvar physxShape = static_cast<PhysXShape*>(shape);
    actor->attachShape(*physxShape->shape);

    // Set default collision
    SetCollisionLayer(uint32_t(CollisionLayers::Default));
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
    actor->setGlobalPose(PxTransform(ToPxV3(position), pose.q));
}

void PhysXActor::SetRotation(const Quaternion& rotation)
{
    cvar pose = actor->getGlobalPose();
    actor->setGlobalPose(PxTransform(pose.p, ToPxQ(rotation)));
}

void PhysXActor::AddForce(const Vector3& force, ForceMode forceMode, const bool awake)
{
    ASSERT(m_dynamic);

    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->addForce(ToPxV3(force), static_cast<PxForceMode::Enum>(forceMode), awake);
}

void PhysXActor::AddTorque(const Vector3& torque, ForceMode forceMode, const bool awake)
{
    ASSERT(m_dynamic);

    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->addTorque(ToPxV3(torque), static_cast<PxForceMode::Enum>(forceMode), awake);
}

Vector3 PhysXActor::GetPosition()
{
    var pose = actor->getGlobalPose();
    rvar pos = pose.p;

    return FromPxV3(pos);
}

Quaternion PhysXActor::GetRotation()
{
    var pose = actor->getGlobalPose();
    rvar rot = pose.q;

    return FromPxQ(rot);
}

void PhysXActor::SetVelocity(const Vector3& velocity)
{
    ASSERT(m_dynamic);

    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->setLinearVelocity(ToPxV3(velocity));
}

Vector3 PhysXActor::GetVelocity()
{
    ASSERT(m_dynamic);

    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    cvar vel = dynamic->getLinearVelocity();
    return FromPxV3(vel);
}

void PhysXActor::SetCollisionLayer(const uint32_t layer)
{
    // Create filter data
    var filter = PxFilterData();
    filter.word0 = layer;

    cvar shapeCount = actor->getNbShapes();

    if (shapeCount == 0u)
        return;

    // Set this layer for all children shapes
    PxShape* shape;
    for (var i = 0u; i < shapeCount; i++)
    {
        ASSERT(actor->getShapes(&shape, 1, i) == 1);
        shape->setQueryFilterData(filter);
        shape->setSimulationFilterData(filter);
    }
}
