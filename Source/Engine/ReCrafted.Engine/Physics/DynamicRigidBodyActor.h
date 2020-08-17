// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Physics/RigidBodyActor.h"

/// <summary>
///     DynamicRigidBodyActor actor class.
/// </summary>
API_CLASS(public, sealed)
class DynamicRigidBodyActor final : public RigidBodyActor
{
    API_CLASS_BODY()

protected:
    void OnAwake() override;

public:
    ACTOR_BODY(DynamicRigidBodyActor)
};
