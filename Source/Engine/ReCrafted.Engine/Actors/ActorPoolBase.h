// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Actors/ActorBase.h"

class ActorPoolBase
{
    friend class ActorPoolManager;

public:
    ActorPoolBase() = default;
    virtual ~ActorPoolBase() = default;

protected:
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual void Refresh() = 0;

public:
    void EventAcquire(ActorBase* actor)
    {
        actor->OnAcquire();
    }

    void EventRelease(ActorBase* actor)
    {
        actor->OnRelease();
    }

    void InitializeActor(ActorBase* actor, const ActorId_t id)
    {
        actor->Cleanup(id);
    }

    void CleanupActor(ActorBase* actor)
    {
        ASSERT(actor);
        actor->Cleanup();
    }
};
