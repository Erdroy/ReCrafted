// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXCharacter.h"
#include "Common/Time.h"

extern PxPhysics* GPxPhysX;

IPhysicsCharacter::CollisionFlags PhysXCharacter::Move(const Vector3& vector)
{
    // Create filters
    PxControllerFilters filters;
    filters.mFilterFlags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER;
    
    var flags = m_controller->move(ToPxV3(vector), 0.001f, float(Time::FixedDeltaTime()), filters);
    return *reinterpret_cast<CollisionFlags*>(&flags);
}

void PhysXCharacter::SetUpDirection(const Vector3& upDirection)
{
    m_controller->setUpDirection(ToPxV3(upDirection));
}

Vector3 PhysXCharacter::GetUpDirection()
{
    return FromPxV3(m_controller->getUpDirection());
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
    return FromPxV3(m_controller->getActor()->getLinearVelocity());
}

void PhysXCharacter::SetPosition(const Vector3& position)
{
    m_controller->setPosition(ToPxV3Ex(position));
}

Vector3 PhysXCharacter::GetPosition()
{
    return FromPxV3Ex(m_controller->getPosition());
}

void PhysXCharacter::SetRotation(const Quaternion& rotation)
{
    cvar actor = m_controller->getActor();
    actor->setGlobalPose(PxTransform(actor->getGlobalPose().p, ToPxQ(rotation)));
}

Quaternion PhysXCharacter::GetRotation()
{
    return FromPxQ(m_controller->getActor()->getGlobalPose().q);
}

void PhysXCharacter::SetCollisionLayer(const uint32_t layer)
{
    // Create filter data
    var filter = PxFilterData();
    filter.word0 = layer;

    cvar shapeCount = m_controller->getActor()->getNbShapes();

    if (shapeCount == 0u)
        return;

    // Set this layer for all children shapes
    PxShape* shape;
    for (var i = 0u; i < shapeCount; i++)
    {
        ASSERT(m_controller->getActor()->getShapes(&shape, 1, i) == 1);
        shape->setQueryFilterData(filter);
        shape->setSimulationFilterData(filter);
    }
}
