// ReCrafted (c) 2016-2019 Always Too Late

#include "RigidBodyActor.h"
#include "PhysicsManager.h"

void RigidBodyActor::Initialize()
{
    ASSERT(m_actor == nullptr);

    m_actor = PhysicsManager::Engine()->CreateActor(GetTransform(), m_dynamic);
    _ASSERT_(m_actor != nullptr, "Failed to create RigidBodyActor");

    PhysicsManager::Scene()->AttachActor(m_actor);
}

void RigidBodyActor::OnStart()
{
    if (m_actor == nullptr)
        Initialize();
}

void RigidBodyActor::OnUpdate()
{
    ASSERT(m_actor);

    Super::SetPosition(m_actor->GetPosition());
    Super::SetRotation(m_actor->GetRotation());

    // TODO: Interpolation/Extrapolation
}

void RigidBodyActor::OnSimulate()
{
    ASSERT(m_actor);

    if(m_dynamic)
    {
        // Calculate gravity
        m_gravity = Vector3::Normalize(GetPosition()) * -9.81f; // TODO: Use gravitational fields

        // Apply gravity (Add force)
        m_actor->AddForce(m_gravity, ForceMode::Acceleration);
    }
}

void RigidBodyActor::OnDestroy()
{
    if(PhysicsManager::Scene())
        PhysicsManager::Scene()->DetachActor(m_actor);

    ASSERT(m_actor);

    if(m_collision)
    {
        m_actor->DetachShape(m_collision->m_shape);
        Object::Destroy(m_collision);
        m_collision = nullptr;
    }

    PhysicsManager::Engine()->ReleaseActor(m_actor);
    m_actor = nullptr;
}

void RigidBodyActor::AttachCollision(Collision* collision)
{
    MAIN_THREAD_ONLY();
    ASSERT(m_actor);
    ASSERT(collision);
    ASSERT(collision->m_shape);
    ASSERT(collision->m_attached == false);
    ASSERT(m_collision == nullptr);

    collision->m_attached = true;
    m_actor->AttachShape(collision->m_shape);
    m_collision = collision;
}

void RigidBodyActor::DetachCollision()
{
    MAIN_THREAD_ONLY();
    ASSERT(m_actor);
    ASSERT(m_collision != nullptr);

    m_collision->m_attached = false;
    m_actor->DetachShape(m_collision->m_shape);
    m_collision = nullptr;
}

void RigidBodyActor::SetPosition(const Vector3& position)
{
    Super::SetPosition(position);

    // Update actor transform
    m_actor->SetPosition(position);
}

void RigidBodyActor::SetRotation(const Quaternion& rotation)
{
    Super::SetRotation(rotation);

    // Update actor transform
    m_actor->SetRotation(rotation);
}

void RigidBodyActor::AddForce(const Vector3& force, const ForceMode forceMode, const bool awake) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->AddForce(force, forceMode, awake);
}

void RigidBodyActor::AddTorque(const Vector3& torque, const ForceMode forceMode, const bool awake) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->AddTorque(torque, forceMode, awake);
}

void RigidBodyActor::SetCollisionLayer(const uint32_t layer) const
{
    MAIN_THREAD_ONLY();
    m_actor->SetCollisionLayer(layer);
}

uint32_t RigidBodyActor::GetCollisionLayer() const
{
    MAIN_THREAD_ONLY();
    return m_actor->GetCollisionLayer();
}

void RigidBodyActor::SetVelocity(const Vector3& velocity) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->SetVelocity(velocity);
}

Vector3 RigidBodyActor::GetVelocity() const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    return m_actor->GetVelocity();
}

void RigidBodyActor::SetAngularVelocity(const Vector3& angularVelocity) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->SetAngularVelocity(angularVelocity);
}

Vector3 RigidBodyActor::GetAngularVelocity() const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    return m_actor->GetAngularVelocity();
}

void RigidBodyActor::SetCentreOfMass(const Vector3& massCentre) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->SetCentreOfMass(massCentre);
}

Vector3 RigidBodyActor::GetCentreOfMass() const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    return m_actor->GetCentreOfMass();
}

void RigidBodyActor::SetMaxAngularVelocity(const float maxAngularVelocity) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->SetMaxAngularVelocity(maxAngularVelocity);
}

float RigidBodyActor::GetMaxAngularVelocity() const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    return m_actor->GetMaxAngularVelocity();
}

void RigidBodyActor::SetLinearDamping(const float damping) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->SetLinearDamping(damping);
}

float RigidBodyActor::GetLinearDamping() const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    return m_actor->GetLinearDamping();
}

void RigidBodyActor::SetAngularDamping(const float angularDamping) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->SetAngularDamping(angularDamping);
}

float RigidBodyActor::GetAngularDamping() const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    return m_actor->GetAngularDamping();
}

void RigidBodyActor::SetMass(const float mass) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->SetMass(mass);
}

float RigidBodyActor::GetMass() const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    return m_actor->GetMass();
}

bool RigidBodyActor::IsDynamic() const
{
    MAIN_THREAD_ONLY();
    return m_dynamic;
}

void RigidBodyActor::IsSleeping(const bool sleep) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->IsSleeping(sleep);
}

bool RigidBodyActor::IsSleeping() const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    return m_actor->IsSleeping();
}

void RigidBodyActor::SetCCD(const bool enabled) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->SetCCD(enabled);
}

bool RigidBodyActor::GetCCD() const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    return m_actor->GetCCD();
}

void RigidBodyActor::IsKinematic(const bool isKinematic) const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    m_actor->IsKinematic(isKinematic);
}

bool RigidBodyActor::IsKinematic() const
{
    MAIN_THREAD_ONLY();
    ASSERT(m_dynamic);
    return m_actor->IsKinematic();
}

RigidBodyActor* RigidBodyActor::Create()
{
    MAIN_THREAD_ONLY();
    return CreateDynamic();
}

RigidBodyActor* RigidBodyActor::CreateDynamic()
{
    MAIN_THREAD_ONLY();
    var actor = Super::Create();
    actor->m_dynamic = true;
    actor->Initialize();
    return actor;
}

RigidBodyActor* RigidBodyActor::CreateStatic()
{
    MAIN_THREAD_ONLY();
    var actor = Super::Create();
    actor->m_dynamic = false;
    actor->Initialize();
    return actor;
}
