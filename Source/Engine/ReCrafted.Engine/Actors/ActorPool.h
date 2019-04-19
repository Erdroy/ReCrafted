// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Actors/ActorPoolBase.h"

#include <concurrentqueue.h>

template<typename TActor>
class ActorPool : public ActorPoolBase
{
private:
    List<TActor*> m_actors;
    moodycamel::ConcurrentQueue<TActor*> m_actorPool;
    moodycamel::ConcurrentQueue<TActor*> m_releaseQueue;

private:
    void AllocateActors(const size_t numActors)
    {
        for (auto i = 0u; i < numActors; i++)
        {
            auto actor = new TActor();
            
            InitializeActor(actor, i);
            
            m_actorPool.enqueue(actor);
            m_actors.Add(actor);
        }
    }

public:
    void Initialize() override
    {
        AllocateActors(ACTOR_POOL_INITIAL_COUNT);
    }

    void Shutdown() override
    {
        auto id = 0;
        for (auto&& actor : m_actors)
        {
            delete actor;
            id++;
        }
        m_actors.Clear();
    }

    void Refresh() override
    {
        TActor* actor;
        while (m_releaseQueue.try_dequeue(actor))
        {
            // Cleanup actor
            CleanupActor(actor);

            // Return actor to the queue
            m_actorPool.enqueue(actor);
        }
    }

public:
    TActor* AcquireActor()
    {
        TActor* actor;
        if (!m_actorPool.try_dequeue(actor))
        {
            // Allocate actors
            AllocateActors(ACTOR_POOL_INCREMENT_COUNT);

            // This should never fail - Sanity check
            ASSERT(m_actorPool.try_dequeue(actor));
            //ASSERT(Object::IsObjectInitialized(actor) == false);
        }

        // Create managed instance
        //Object::CreateInstance(actor, TActor::ManagedNamespace, TActor::ManagedName);

        // Call acquire event
        EventAcquire(actor);

        return actor;
    }

    void ReleaseActor(TActor* actor)
    {
        //ASSERT(Object::IsObjectInitialized(actor));

        // Call release event
        EventRelease(actor);

        // Return actor to pool
        m_actorPool.enqueue(actor);
    }
};
