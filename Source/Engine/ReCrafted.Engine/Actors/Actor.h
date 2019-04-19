// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Actors/ActorBase.h"
#include "Actors/ActorPool.h"
#include "Actors/ActorPoolManager.h"

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
        MAIN_THREAD_ONLY();
        ASSERT(ActorPoolManager::GetPool<TActor>()); // Sanity check
        return ActorPoolManager::GetPool<TActor>()->AcquireActor();
    }

    static void Destroy(TActor* actor)
    {
        MAIN_THREAD_ONLY();
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

    for (auto&& child : GetChildren())
        Destroy(static_cast<TActor*>(child));
}
