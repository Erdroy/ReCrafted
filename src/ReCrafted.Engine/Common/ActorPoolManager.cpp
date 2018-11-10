// ReCrafted (c) 2016-2018 Always Too Late

#include "ActorPoolManager.h"
#include "Profiler/Profiler.h"

SINGLETON_IMPL(ActorPoolManager)

void ActorPoolManager::RefreshActorPools()
{
    Profiler::BeginProfile(__FUNCTION__);

    for (rvar pool : m_actorPools)
    {
        ASSERT(pool);
        pool->Refresh();
    }

    Profiler::EndProfile();
}

void ActorPoolManager::OnInit()
{
    m_actorPools = {};
}

void ActorPoolManager::OnDispose()
{
    for (rvar pool : m_actorPools)
    {
        pool->Shutdown();
    }
    m_actorPools.Clear();
}

void ActorPoolManager::OnFrameFinished()
{
    RefreshActorPools();
}

void ActorPoolManager::Register(ActorPoolBase* pool)
{
    ASSERT(pool);
    pool->Initialize();
    m_instance->m_actorPools.Add(pool);
}
