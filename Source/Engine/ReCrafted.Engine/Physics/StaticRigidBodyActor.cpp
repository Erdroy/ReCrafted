// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "StaticRigidBodyActor.h"
#include "PhysicsManager.h"

PxRigidActor* StaticRigidBodyActor::CreatePxActor(const PxTransform& transform)
{
    return PhysicsManager::GetPhysics()->createRigidStatic(transform);
}
