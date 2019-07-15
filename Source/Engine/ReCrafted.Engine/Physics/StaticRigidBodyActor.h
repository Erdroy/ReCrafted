// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Physics/RigidBodyActor.h"

/// <summary>
///     StaticRigidBodyActor actor class.
/// </summary>
API_CLASS(public, sealed)
class StaticRigidBodyActor final : public RigidBodyActor
{
    API_CLASS_BODY()

protected:
    void OnAwake() override;

public:
    ACTOR_BODY(StaticRigidBodyActor)
};
