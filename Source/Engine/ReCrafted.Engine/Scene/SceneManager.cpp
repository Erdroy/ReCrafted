// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "SceneManager.h"
#include "Core/Actors/ActorBase.h"

void SceneManager::Initialize()
{
}

void SceneManager::Shutdown()
{
    if (!m_actors.Empty())
    {
        // Destroy all actors
        for (auto&& actor : m_actors)
            Object::DestroyNow(actor);

        m_actors.Clear();
    }
}

void SceneManager::OnUpdate()
{
    for (auto&& actor : m_actors)
    {
        if (actor->IsActive())
            actor->Update();
    }
}

void SceneManager::OnLateUpdate()
{
    for (auto&& actor : m_actors)
    {
        if (actor->IsActive())
            actor->LateUpdate();
    }
}

void SceneManager::OnFixedUpdate()
{
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
