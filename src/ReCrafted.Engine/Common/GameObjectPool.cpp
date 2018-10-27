// ReCrafted (c) 2016-2018 Always Too Late

#include "GameObjectPool.h"
#include "Core/Logger.h"
#include "Profiler/Profiler.h"

SINGLETON_IMPL(GameObjectPool)

void GameObjectPool::AllocateGameObjects(const size_t numGameObjects)
{
    for(var i = 0u; i < numGameObjects; i ++)
    {
        var gameObject = Object::CreateInstance<GameObject>("ReCrafted.API.Common", "GameObject");
        m_freeGameObjects.enqueue(gameObject);
        m_gameObjects.Add(gameObject);
    }
}

void GameObjectPool::CleanupGameObject(GameObject* gameObject)
{
    ASSERT(gameObject);

    // TODO: Cleanup gameObject
}

void GameObjectPool::OnInit()
{
    // Allocate gameObjects
    AllocateGameObjects(GAMEOBJECT_POOL_INITIAL_COUNT);

    Logger::Log("Initialized GameObject Pool with {0} GameObjects.", GAMEOBJECT_POOL_INITIAL_COUNT);
}

void GameObjectPool::OnDispose()
{
    // Cleanup
    for(rvar gameObject : m_gameObjects)
        delete gameObject;

    m_gameObjects.Clear();
}

void GameObjectPool::Update()
{
}

void GameObjectPool::LateUpdate()
{
    // TODO: Activate/Deactivate game objects etc.
}

void GameObjectPool::OnFrameFinished()
{
    UpdateGameObjectPool();
}

void GameObjectPool::UpdateGameObjectPool()
{
    Profiler::BeginProfile(__FUNCTION__);

    GameObject* gameObject;
    while(m_releaseGameObjects.try_dequeue(gameObject))
    {
        // Cleanup gameObject
        CleanupGameObject(gameObject);

        // Return this gameObject to the queue
        m_freeGameObjects.enqueue(gameObject);
    }

    Profiler::EndProfile();
}

GameObject* GameObjectPool::AcquireGameObject()
{
    GameObject* gameObject;
    if (m_instance->m_freeGameObjects.try_dequeue(gameObject))
        return gameObject;
    
    // Allocate gameObject
    m_instance->AllocateGameObjects(GAMEOBJECT_POOL_INCREMENT_COUNT);

    // Shis should never fail
    ASSERT(m_instance->m_freeGameObjects.try_dequeue(gameObject));
    return gameObject;
}

void GameObjectPool::ReleaseGameObject(GameObject* gameObject)
{
    // Enqueue gameObject
    m_instance->m_releaseGameObjects.enqueue(gameObject);
}
