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

    for(var gameObject : m_gameObjects)
        gameObject->Update();

    Profiler::EndProfile();
}

void SceneManager::LateUpdate()
{
    Profiler::BeginProfile(__FUNCTION__);

    for (var gameObject : m_gameObjects)
        gameObject->LateUpdate();

    Profiler::EndProfile();
}

void SceneManager::Simulate()
{
    //Profiler::BeginProfile(__FUNCTION__);

    for (var gameObject : m_gameObjects)
        gameObject->Simulate();

    //Profiler::EndProfile();
}
