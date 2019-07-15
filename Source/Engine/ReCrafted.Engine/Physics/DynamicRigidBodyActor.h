// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Physics/RigidBodyActor.h"

/// <summary>
///     DynamicRigidBodyActor actor class.
/// </summary>
API_CLASS(public, sealed)
class DynamicRigidBodyActor final : public RigidBodyActor
{
protected:
    PxRigidActor* CreatePxActor(const PxTransform& transform) override;
};
