// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "CharacterActor.h"
#include "Core/Time.h"
#include "PhysicsMaterial.h"
#include "PhysicsManager.h"

CharacterActor::~CharacterActor()
{
    PX_RELEASE(m_controller);
}

void CharacterActor::OnAwake()
{
    // Calculate unity-like height value
    const auto finalHeight = PxClamp((m_height - 2 * m_radius), 0.0f, m_height);

    // Create controller description
    PxCapsuleControllerDesc controllerDesc;
    controllerDesc.radius = m_radius;
    controllerDesc.height = finalHeight;
    controllerDesc.stepOffset = m_stepOffset;
    controllerDesc.slopeLimit = Math::IsZero(m_slopeLimit) ? 0.0f : Math::Cos(m_slopeLimit * Math::DegreeToRadian);
    controllerDesc.contactOffset = m_contactOffset;
    controllerDesc.material = PhysicsMaterial::Default()->GetPxMaterial();

    // Make sure that we've got proper controller description
    ASSERT(controllerDesc.isValid());

    // Get scene
    const auto scene = PhysicsManager::GetSceneAt(Vector3::Zero);
    const auto controllerManager = scene->GetPxControllerManager();

    // Create controller
    m_controller = static_cast<PxCapsuleController*>(controllerManager->createController(controllerDesc));

    // Set default layers
    CollisionLayer(0u); // TODO: Builtin layers (eg. Character layer etc.)
}

void CharacterActor::OnUpdate()
{
    const auto position = FromPxV3Ex(m_controller->getPosition());
    ActorBase::Position(position);
}

void CharacterActor::Position(const Vector3& position)
{
    ActorBase::Position(position);

    // Update actor transform
    m_controller->setPosition(ToPxV3Ex(position));
}

void CharacterActor::Rotation(const Quaternion& rotation)
{
    const auto actor = m_controller->getActor();
    actor->setGlobalPose(PxTransform(actor->getGlobalPose().p, ToPxQ(rotation)));
}

CharacterCollisionFlags CharacterActor::Move(const Vector3& displacement)
{
    ASSERT(m_controller);

    // Create filters
    PxControllerFilters filters;
    filters.mFilterFlags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER;

    auto pxFlags = m_controller->move(ToPxV3(displacement), 0.001f, float(Time::FixedDeltaTime()), filters);
    auto flags = *reinterpret_cast<CharacterCollisionFlags*>(&pxFlags);

    // Set grounded flag
    m_grounded = (static_cast<uint>(flags) & static_cast<uint>(CharacterCollisionFlags::Down)) != 0;

    return flags;
}

void CharacterActor::CollisionLayer(const uint32_t layer)
{
    m_collisionLayer = layer;

    // Create filter data
    auto filter = PxFilterData();
    filter.word0 = layer;

    const auto shapeCount = m_controller->getActor()->getNbShapes();

    if (shapeCount == 0u)
        return;

    // Set this layer for all children shapes
    PxShape* shape;
    for (auto i = 0u; i < shapeCount; i++)
    {
        ASSERT(m_controller->getActor()->getShapes(&shape, 1, i) == 1);
        shape->setQueryFilterData(filter);
        shape->setSimulationFilterData(filter);
    }
}

uint32_t CharacterActor::CollisionLayer() const
{
    return m_collisionLayer;
}

void CharacterActor::UpDirection(const Vector3& direction) const
{
    m_controller->setUpDirection(ToPxV3(direction));
}

Vector3 CharacterActor::UpDirection() const
{
    return FromPxV3(m_controller->getUpDirection());
}

void CharacterActor::Height(const float height)
{
    m_height = height;
    m_controller->resize(height);
}

float CharacterActor::Height() const
{
    return m_height;
}

void CharacterActor::Radius(const float radius)
{
    m_radius = radius;
    m_controller->setRadius(radius);
}

float CharacterActor::Radius() const
{
    return m_radius;
}

void CharacterActor::StepOffset(const float stepOffset)
{
    m_stepOffset = stepOffset;
    m_controller->setStepOffset(stepOffset);
}

float CharacterActor::StepOffset() const
{
    return m_stepOffset;
}

void CharacterActor::ContactOffset(const float contactOffset)
{
    m_contactOffset = contactOffset;
    m_controller->setContactOffset(contactOffset);
}

float CharacterActor::ContactOffset() const
{
    return m_contactOffset;
}

void CharacterActor::SlopeLimit(const float slopeLimit)
{
    if (Math::IsZero(slopeLimit))
    {
        m_slopeLimit = 0.0f;
        m_controller->setSlopeLimit(0.0f);
    }
    else
    {
        m_slopeLimit = slopeLimit;
        m_controller->setSlopeLimit(Math::Cos(slopeLimit * Math::DegreeToRadian));
    }
}

float CharacterActor::SlopeLimit() const
{
    return m_slopeLimit;
}

Vector3 CharacterActor::Velocity() const
{
    return FromPxV3(m_controller->getActor()->getLinearVelocity());
}

bool CharacterActor::IsGrounded() const
{
    return m_grounded;
}
