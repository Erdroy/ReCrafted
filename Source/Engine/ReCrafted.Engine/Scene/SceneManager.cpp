// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "SceneManager.h"
#include "Core/Actors/ActorBase.h"
#include "Profiler/Profiler.h"

void SceneManager::Initialize()
{
}

void SceneManager::Shutdown()
{
    if (!m_actors.Empty())
    {
        // Destroy all actors
        List<ActorBase*> actors;
        actors.Copy(m_actors);

        for (auto&& actor : actors)
            Object::DestroyNow(actor);

        m_actors.Clear();
    }
}

void SceneManager::OnUpdate()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    for (auto&& actor : m_actors)
    {
        if (actor->IsActive())
            actor->Update();
    }
}

void SceneManager::OnLateUpdate()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    for (auto&& actor : m_actors)
    {
        if (actor->IsActive())
            actor->LateUpdate();
    }
}

void SceneManager::OnFixedUpdate()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    for (auto&& actor : m_actors)
    {
        if (actor->IsActive())
            actor->FixedUpdate();
    }
}

void SceneManager::AddActor(ActorBase* actor)
{
    MAIN_THREAD_ONLY();
    m_actors.Add(actor);

    actor->OnAwake();
}

void SceneManager::RemoveActor(ActorBase* actor)
{
    MAIN_THREAD_ONLY();
    m_actors.Remove(actor);
}
