// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ACTORPOOL_H
#define ACTORPOOL_H

#include "ReCrafted.h"
#include "Common/ActorBase.h"
#include "Core/Containers/Array.h"
#include "Scripting/Object.h"

#include <concurrentqueue.h>

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

protected:
    void Register(ActorPoolBase* pool);

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

template<typename TActor>
class ActorPool : public ActorPoolBase
{
private:
    Array<TActor*> m_actors;
    moodycamel::ConcurrentQueue<TActor*> m_actorPool;
    moodycamel::ConcurrentQueue<TActor*> m_releaseQueue;

private:
    void AllocateActors(const size_t numActors)
    {
        for (var i = 0u; i < numActors; i++)
        {
            var actor = new TActor();
            
            InitializeActor(actor, i);
            
            m_actorPool.enqueue(actor);
            m_actors.Add(actor);
        }
    }

public:
    ActorPool()
    {
        // Register
        Register(this);
    }

    ~ActorPool()
    {
        // Unregister
    }

public:
    void Initialize() override
    {
        AllocateActors(ACTOR_POOL_INITIAL_COUNT); // TODO: Per pool settings
    }

    void Shutdown() override
    {
        var id = 0;
        for (rvar actor : m_actors)
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
            ASSERT(Object::IsObjectInitialized(actor) == false);
        }

        // Create managed instance
        Object::CreateInstance(actor, TActor::ManagedNamespace, TActor::ManagedName);

        // Call acquire event
        EventAcquire(actor);

        return actor;
    }

    void ReleaseActor(TActor* actor)
    {
        ASSERT(Object::IsObjectInitialized(actor));

        // Call release event
        EventRelease(actor);

        // Return actor to pool
        m_actorPool.enqueue(actor);
    }
};

#endif // ACTORPOOL_H
