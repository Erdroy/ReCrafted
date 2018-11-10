// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ACTOR_H
#define ACTOR_H

#include "ReCrafted.h"
#include "Common/ActorBase.h"
#include "Common/ActorPool.h"
#include "Common/ActorPoolManager.h"

template<typename TActor>
class Actor : public ActorBase
{
    friend class ActorPool<TActor>;

public:
    using Super = Actor<TActor>;

public:
    Actor() = default;
    ~Actor() = default;

public:
    void Destroy() override;

public:
    static TActor* Create()
    {
        ASSERT(ActorPoolManager::GetPool<TActor>()); // Sanity check
        return ActorPoolManager::GetPool<TActor>()->AcquireActor();
    }

    static void Destroy(TActor* actor)
    {
        ASSERT(ActorPoolManager::GetPool<TActor>());
        ASSERT(actor);
        ActorPoolManager::GetPool<TActor>()->ReleaseActor(actor);
    }

protected:
    static const char* ManagedName;
    static const char* ManagedNamespace;
};

template <typename TActor>
void Actor<TActor>::Destroy()
{
    Destroy(static_cast<TActor*>(this));

    for (rvar child : GetChildren())
        Destroy(static_cast<TActor*>(child));
}

#endif // ACTOR_H
