// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/Actors/ActorBase.h"

class EmptyActor final : public ActorBase
{
public:
    static EmptyActor* Create()
    {
        return New<EmptyActor>();
    }
};
