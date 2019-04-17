// ReCrafted (c) 2016-2019 Always Too Late

#include "ActorPoolManager.h"
//#include "Common/Profiler/Profiler.h"

void ActorPoolManager::RefreshActorPools()
{
    //Profiler::BeginProfile(__FUNCTION__);

    for (auto&& pool : m_actorPools)
    {
        ASSERT(pool);
        pool->Refresh();
    }

    //Profiler::EndProfile();
}

void ActorPoolManager::Initialize()
{
    m_actorPools = {};
}

void ActorPoolManager::Shutdown()
{
    for (auto&& pool : m_actorPools)
    {
        pool->Shutdown();

        // Delete pool, call it's destructor
        delete pool;
    }
    m_actorPools.Clear();
}

void ActorPoolManager::OnFrameDone()
{
    RefreshActorPools();
}

void ActorPoolManager::Register(ActorPoolBase* pool)
{
    ASSERT(pool);
    pool->Initialize();
    GetInstance()->m_actorPools.Add(pool);
}
