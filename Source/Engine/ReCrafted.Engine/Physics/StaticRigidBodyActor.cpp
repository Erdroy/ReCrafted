// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "StaticRigidBodyActor.h"
#include "PhysicsManager.h"

void StaticRigidBodyActor::OnAwake()
{
    m_dynamic = false;

    auto& position = GetTransform().translation;
    auto& rotation = GetTransform().orientation;

    const auto pxTransform = PxTransform(position.x, position.y, position.z, PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
    ASSERT(pxTransform.isValid());

    m_actor = PhysicsManager::GetPhysics()->createRigidStatic(pxTransform);
    ASSERT(m_actor);
}
