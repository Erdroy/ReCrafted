// ReCrafted (c) 2016-2018 Always Too Late

#include "SceneManager.h"
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(SceneManager)

void SceneManager::OnInit()
{

}

void SceneManager::OnDispose()
{
}

void SceneManager::Update()
{
    Profiler::BeginProfile(__FUNCTION__);

    for (var actor : m_actors)
    {
        if (actor->IsActive())
            actor->Update();
    }

    Profiler::EndProfile();
}

void SceneManager::LateUpdate()
{
    Profiler::BeginProfile(__FUNCTION__);

    for (var actor : m_actors)
    {
        if(actor->IsActive())
            actor->LateUpdate();
    }

    Profiler::EndProfile();
}

void SceneManager::Simulate()
{
    //Profiler::BeginProfile(__FUNCTION__);

    for (var actor : m_actors)
    {
        if (actor->IsActive())
            actor->Simulate();
    }

    //Profiler::EndProfile();
}

void SceneManager::AddActor(ActorBase* actor)
{
    m_actors.Remove(actor);
}

void SceneManager::RemoveActor(ActorBase* actor)
{
    m_actors.Remove(actor);
}
