// ReCrafted (c) 2016-2018 Always Too Late

#include "ActorPool.h"
#include "Core/Logger.h"
#include "Profiler/Profiler.h"

SINGLETON_IMPL(ActorPool)

void ActorPool::AllocateActors(const size_t numActors)
{
    for(var i = 0u; i < numActors; i ++)
    {
        var actor = Object::CreateInstance<Actor>("ReCrafted.API.Common", "Actor");
        m_freeActors.enqueue(actor);
        m_actors.Add(actor);
    }
}

void ActorPool::CleanupActor(Actor* actor)
{
    ASSERT(actor);

    // TODO: Cleanup actor
}

void ActorPool::OnInit()
{
    // Allocate actors
    AllocateActors(ACTOR_POOL_INITIAL_ACTOR_COUNT);

    Logger::Log("Initialized Actor pool with {0} Actors.", ACTOR_POOL_INITIAL_ACTOR_COUNT);
}

void ActorPool::OnDispose()
{
    // Cleanup
    for(rvar actor : m_actors)
        delete actor;

    m_actors.Clear();
}

void ActorPool::Update()
{
}

void ActorPool::LateUpdate()
{
    // TODO: Activate/Deactivate actors etc.
}

void ActorPool::OnFrameFinished()
{
    UpdateActorPool();
}

void ActorPool::UpdateActorPool()
{
    Profiler::BeginProfile(__FUNCTION__);

    Actor* actor;
    while(m_releaseActors.try_dequeue(actor))
    {
        // Cleanup actor
        CleanupActor(actor);

        // Return this actor to the queue
        m_freeActors.enqueue(actor);
    }

    Profiler::EndProfile();
}

Actor* ActorPool::AcquireActor()
{
    Actor* actor;
    if (m_instance->m_freeActors.try_dequeue(actor))
        return actor;
    
    // Allocate actors
    m_instance->AllocateActors(ACTOR_POOL_INCREMENT_ACTOR_COUNT);

    // Shis should never fail
    ASSERT(m_instance->m_freeActors.try_dequeue(actor));
    return actor;
}

void ActorPool::ReleaseActor(Actor* actor)
{
    // Enqueue actor
    m_instance->m_releaseActors.enqueue(actor);
}
