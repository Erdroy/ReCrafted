// ReCrafted (c) 2016-2018 Always Too Late

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

    SetPosition(m_actor->GetPosition());
    SetRotation(m_actor->GetRotation());

    // TODO: Interpolation/Extrapolation
}

void RigidBodyActor::OnDestroy()
{
    if(PhysicsManager::Scene())
        PhysicsManager::Scene()->DetachActor(m_actor);

    ASSERT(m_actor);

    if(m_collision)
    {
        DetachCollision();
        Object::Destroy(m_collision);
        m_collision = nullptr;
    }

    PhysicsManager::Engine()->ReleaseActor(m_actor);
    m_actor = nullptr;
}

void RigidBodyActor::AttachCollision(Collision* collision)
{
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

RigidBodyActor* RigidBodyActor::Create()
{
    return CreateDynamic();
}

RigidBodyActor* RigidBodyActor::CreateDynamic()
{
    var actor = Super::Create();
    actor->m_dynamic = true;
    actor->Initialize();
    return actor;
}

RigidBodyActor* RigidBodyActor::CreateStatic()
{
    var actor = Super::Create();
    actor->m_dynamic = false;
    actor->Initialize();
    return actor;
}
