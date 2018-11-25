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

void PhysXActor::SetAngularVelocity(const Vector3& angularVelocity)
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->setAngularVelocity(ToPxV3(angularVelocity));
}

Vector3 PhysXActor::GetAngularVelocity()
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    return FromPxV3(dynamic->getAngularVelocity());
}

void PhysXActor::SetCentreOfMass(const Vector3& massCentre)
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->setCMassLocalPose(PxTransform(ToPxV3(massCentre)));
}

Vector3 PhysXActor::GetCentreOfMass()
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    return FromPxV3(dynamic->getCMassLocalPose().p);
}

void PhysXActor::SetMaxAngularVelocity(const float maxAngularVelocity)
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->setMaxAngularVelocity(maxAngularVelocity);
}

float PhysXActor::GetMaxAngularVelocity()
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    return dynamic->getMaxAngularVelocity();
}

void PhysXActor::SetLinearDamping(const float damping)
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->setLinearDamping(damping);
}

float PhysXActor::GetLinearDamping()
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    return dynamic->getLinearDamping();
}

void PhysXActor::SetAngularDamping(const float angularDamping)
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->setAngularDamping(angularDamping);
}

float PhysXActor::GetAngularDamping()
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    return dynamic->getAngularDamping();
}

void PhysXActor::SetMass(const float mass)
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->setMass(mass);
}

float PhysXActor::GetMass()
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    return dynamic->getMass();
}

bool PhysXActor::IsDynamic()
{
    return m_dynamic;
}

void PhysXActor::IsSleeping(const bool sleep)
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);

    if(dynamic->isSleeping() != sleep)
    {
        if (sleep)
            dynamic->putToSleep();
        else
            dynamic->wakeUp();
    }
}

bool PhysXActor::IsSleeping()
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    return dynamic->isSleeping();
}

void PhysXActor::SetCCD(const bool enabled)
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, enabled);
}

bool PhysXActor::GetCCD()
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    return dynamic->getRigidBodyFlags() & PxRigidBodyFlag::eENABLE_CCD;
}

void PhysXActor::IsKinematic(const bool isKinematic)
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    dynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
}

bool PhysXActor::IsKinematic()
{
    ASSERT(m_dynamic);
    cvar dynamic = static_cast<PxRigidDynamic*>(actor);
    return dynamic->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC;
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
