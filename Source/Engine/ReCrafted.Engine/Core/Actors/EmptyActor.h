// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/Actors/ActorBase.h"

/// <summary>
///     EmptyActor - provides an actor that has no additional functionality.
/// </summary>
API_CLASS(public)
class EmptyActor final : public ActorBase
{
    API_CLASS_BODY()

public:

    /// <summary>
    ///     Creates new actor of type <see cref="EmptyActor"/>.
    /// </summary>
    API_FUNCTION()
    static EmptyActor* Create()
    {
        return New<EmptyActor>();
    }
};
