// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "RigidBodyActor.h"
#include "Core/Time.h"
#include "PhysicsManager.h"
#include "DynamicRigidBodyActor.h"
#include "StaticRigidBodyActor.h"
#include "PhysicsScene.h"

RigidBodyActor::~RigidBodyActor()
{
    PX_RELEASE(m_actor);
}

void RigidBodyActor::OnStart()
{
    // Temporary: Add actor to proper scene TODO: Use world clustering
    const auto scene = PhysicsManager::GetSceneAt(Vector3::Zero);
    ASSERT(scene);
    scene->AttachActor(this);
}

void RigidBodyActor::OnUpdate()
{
    // TODO: Sync could be actually done using DOTS

    switch (m_syncMode)
    {
    case RigidBodySyncMode::Default:
    {
        ActorBase::Position(FromPxV3(m_currentPxPos));
        ActorBase::Rotation(FromPxQ(m_currentPxRot));
        break;
    }
    case RigidBodySyncMode::Interpolation:
    {
        // Interpolate position
        ActorBase::Position(Vector3::Lerp(FromPxV3(m_lastPxPos), FromPxV3(m_currentPxPos), Time::FrameAlpha()));
        ActorBase::Rotation(Quaternion::Slerp(FromPxQ(m_lastPxRot), FromPxQ(m_currentPxRot), Time::FrameAlpha()));

        break;
    }
    case RigidBodySyncMode::Extrapolation:
    {
        auto lastPosition = FromPxV3(m_lastPxPos);
        lastPosition += Velocity();

        auto currentPosition = FromPxV3(m_currentPxPos);
        currentPosition += Velocity() * Time::FrameAlpha();

        if(Vector3::DistanceSquared(lastPosition, currentPosition) > 1.0f)
        {
            // Snap
            lastPosition = currentPosition;
            ActorBase::Position(currentPosition);
        }
        else
        {
            ActorBase::Position(Vector3::Lerp(lastPosition, currentPosition, Time::FrameAlpha()));
        }

        // TODO: Extrapolate rotation

        ActorBase::Rotation(FromPxQ(m_currentPxRot));
        break;
    }

    case RigidBodySyncMode::Count:
    default: 
        break;
    }
}

void RigidBodyActor::OnSimulate()
{
    ASSERT(m_actor);

    if (m_dynamic && m_affectedByGravity)
    {
        // TODO: This could be done using DOTS

        // Calculate gravity
        auto gDir = Vector3::Normalize(Position());

        if (Math::NearEqual(gDir.LengthSquared(), 0.0f))
            gDir = Vector3::Down;

        m_gravity = gDir * -9.81f; // TODO: Use gravitational fields

        // Apply gravity (Add force)
        AddForce(m_gravity, ForceMode::Acceleration);
    }

    const auto pose = m_actor->getGlobalPose();

    m_lastPxPos = m_currentPxPos;
    m_lastPxRot = m_currentPxRot;

    m_currentPxPos = pose.p;
    m_currentPxRot = pose.q;
}

void RigidBodyActor::OnEnable()
{
    m_actor->setActorFlag(PxActorFlag::Enum::eDISABLE_SIMULATION, false);
}

void RigidBodyActor::OnDisable()
{
    m_actor->setActorFlag(PxActorFlag::Enum::eDISABLE_SIMULATION, true);
}

void RigidBodyActor::AttachCollider(Collider* collider, const bool awake)
{
    ASSERT(collider);
    _ASSERT_(collider->m_shape, "Collider is not initialized.");

    m_colliders.emplace_back(collider);
    ASSERT(m_actor->attachShape(*collider->m_shape));

    if (awake && IsSleeping())
    {
        IsSleeping(false);
    }
}

void RigidBodyActor::DetachCollider(Collider* collider, const bool awakeOnLostTouch)
{
    ASSERT(collider);
    _ASSERT_(collider->m_shape, "Collider is not initialized.");

    const auto it = std::find(m_colliders.begin(), m_colliders.end(), collider);
    if(it != m_colliders.end())
    {
        m_actor->detachShape(*collider->m_shape, awakeOnLostTouch);
        m_colliders.erase(it);
    }
}

void RigidBodyActor::DetachColliders(const bool awakeOnLostTouch)
{
    for(auto& collider : m_colliders)
    {
        m_actor->detachShape(*collider->m_shape, awakeOnLostTouch);
    }

    m_colliders.clear();
}

void RigidBodyActor::AddForce(const Vector3& force, ForceMode forceMode, const bool awake) const
{
    ASSERT(m_dynamic);

    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->addForce(ToPxV3(force), static_cast<PxForceMode::Enum>(forceMode), awake);
}

void RigidBodyActor::AddTorque(const Vector3& torque, ForceMode forceMode, const bool awake) const
{
    ASSERT(m_dynamic);

    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->addTorque(ToPxV3(torque), static_cast<PxForceMode::Enum>(forceMode), awake);
}

PhysicsScene* RigidBodyActor::Scene() const
{
    return m_scene;
}

void RigidBodyActor::SyncMode(const RigidBodySyncMode& mode)
{
    m_syncMode = mode;
}

RigidBodySyncMode RigidBodyActor::SyncMode() const
{
    return m_syncMode;
}

void RigidBodyActor::Position(const Vector3& position)
{
    ActorBase::Position(position);

    const auto pose = m_actor->getGlobalPose();
    m_actor->setGlobalPose(PxTransform(ToPxV3(position), pose.q));
}

const Vector3& RigidBodyActor::Position() const
{
    return ActorBase::Position();
}

void RigidBodyActor::Rotation(const Quaternion& rotation)
{
    ActorBase::Rotation(rotation);

    const auto  pose = m_actor->getGlobalPose();
    m_actor->setGlobalPose(PxTransform(pose.p, ToPxQ(rotation)));
}

const Quaternion& RigidBodyActor::Rotation() const
{
    return ActorBase::Rotation();
}

void RigidBodyActor::AffectedByGravity(const bool gravityEnabled)
{
    m_affectedByGravity = gravityEnabled;
}

bool RigidBodyActor::AffectedByGravity() const
{
    return m_affectedByGravity;
}

void RigidBodyActor::CollisionLayer(const uint32_t layer)
{
    // Store new layer
    m_collisionLayer = layer;

    // Create filter data
    auto filter = PxFilterData();
    filter.word0 = layer;

    const auto shapeCount = m_actor->getNbShapes();

    if (shapeCount == 0u)
        return;

    // Set this layer for all children shapes
    PxShape* shape;
    for (auto i = 0u; i < shapeCount; i++)
    {
        ASSERT(m_actor->getShapes(&shape, 1, i) == 1);

        // Set shape filters
        shape->setQueryFilterData(filter);
        shape->setSimulationFilterData(filter);
    }
}

uint32_t RigidBodyActor::CollisionLayer() const
{
    return m_collisionLayer;
}

void RigidBodyActor::Velocity(const Vector3& velocity) const
{
    ASSERT(m_dynamic);

    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->setLinearVelocity(ToPxV3(velocity));
}

Vector3 RigidBodyActor::Velocity() const
{
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    const auto vel = dynamic->getLinearVelocity();
    return FromPxV3(vel);
}

void RigidBodyActor::AngularVelocity(const Vector3& angularVelocity) const
{
    ASSERT(m_dynamic);
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->setAngularVelocity(ToPxV3(angularVelocity));
}

Vector3 RigidBodyActor::AngularVelocity() const
{
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    return FromPxV3(dynamic->getAngularVelocity());
}

void RigidBodyActor::CentreOfMass(const Vector3& massCentre) const
{
    ASSERT(m_dynamic);
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->setCMassLocalPose(PxTransform(ToPxV3(massCentre)));
}

Vector3 RigidBodyActor::CentreOfMass() const
{
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    return FromPxV3(dynamic->getAngularVelocity());
}

void RigidBodyActor::MaxAngularVelocity(const float maxAngularVelocity) const
{
    ASSERT(m_dynamic);
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->setMaxAngularVelocity(maxAngularVelocity);
}

float RigidBodyActor::MaxAngularVelocity() const
{
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    return dynamic->getMaxAngularVelocity();
}

void RigidBodyActor::LinearDamping(const float damping) const
{
    ASSERT(m_dynamic);
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->setLinearDamping(damping);
}

float RigidBodyActor::LinearDamping() const
{
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    return dynamic->getLinearDamping();
}

void RigidBodyActor::AngularDamping(const float angularDamping) const
{
    ASSERT(m_dynamic);
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->setAngularDamping(angularDamping);
}

float RigidBodyActor::AngularDamping() const
{
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    return dynamic->getAngularDamping();
}

void RigidBodyActor::Mass(const float mass) const
{
    ASSERT(m_dynamic);
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->setMass(mass);
}

float RigidBodyActor::Mass() const
{
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    return dynamic->getMass();
}

void RigidBodyActor::CCD(const bool enabled) const
{
    ASSERT(m_dynamic);
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, enabled);
}

bool RigidBodyActor::CCD() const
{
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    return dynamic->getRigidBodyFlags() & PxRigidBodyFlag::eENABLE_CCD;
}

bool RigidBodyActor::IsDynamic() const
{
    return m_dynamic;
}

void RigidBodyActor::IsSleeping(const bool sleep) const
{
    ASSERT(m_dynamic);
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);

    if (dynamic->isSleeping() != sleep)
    {
        if (sleep)
            dynamic->putToSleep();
        else
            dynamic->wakeUp();
    }
}

bool RigidBodyActor::IsSleeping() const
{
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    return dynamic->isSleeping();
}

void RigidBodyActor::IsKinematic(const bool isKinematic) const
{
    ASSERT(m_dynamic);
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    dynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
}

bool RigidBodyActor::IsKinematic() const
{
    const auto dynamic = static_cast<PxRigidDynamic*>(m_actor);
    return dynamic->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC;
}

RigidBodyActor* RigidBodyActor::Create()
{
    return reinterpret_cast<RigidBodyActor*>(New<DynamicRigidBodyActor>());
}

RigidBodyActor* RigidBodyActor::CreateDynamic()
{
    return reinterpret_cast<RigidBodyActor*>(New<DynamicRigidBodyActor>());
}

RigidBodyActor* RigidBodyActor::CreateStatic()
{
    return static_cast<RigidBodyActor*>(New<StaticRigidBodyActor>());
}
