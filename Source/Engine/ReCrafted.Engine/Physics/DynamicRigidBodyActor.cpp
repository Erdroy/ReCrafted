// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "DynamicRigidBodyActor.h"
#include "PhysicsManager.h"

void DynamicRigidBodyActor::OnAwake()
{
    auto& position = GetTransform().translation;
    auto& rotation = GetTransform().orientation;

    const auto pxTransform = PxTransform(position.x, position.y, position.z, PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
    ASSERT(pxTransform.isValid());

    m_actor = PhysicsManager::GetPhysics()->createRigidDynamic(pxTransform);
    ASSERT(m_actor);

    // Disable internal gravity simulation.
    m_actor->setActorFlag(PxActorFlag::Enum::eDISABLE_GRAVITY, true);

}
