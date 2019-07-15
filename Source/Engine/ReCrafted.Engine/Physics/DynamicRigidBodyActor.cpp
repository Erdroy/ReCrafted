// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "DynamicRigidBodyActor.h"
#include "PhysicsManager.h"

PxRigidActor* DynamicRigidBodyActor::CreatePxActor(const PxTransform& transform)
{
    return PhysicsManager::GetPhysics()->createRigidDynamic(transform);
}
