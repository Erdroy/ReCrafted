// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/Actors/ActorBase.h"

API_CLASS(public)
class EmptyActor final : public ActorBase
{
    API_CLASS_BODY()

public:
    API_FUNCTION();
    static EmptyActor* Create()
    {
        return New<EmptyActor>();
    }
};
