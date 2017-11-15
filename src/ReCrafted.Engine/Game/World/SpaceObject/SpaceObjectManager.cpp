// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObjectManager.h"
#include "SpaceObjectChunk.h"
#include "Core/concurrentqueue.h"

SpaceObjectManager* Singleton<SpaceObjectManager>::m_instance;

//moodycamel::ConcurrentQueue<> m_loadingQueue;
//moodycamel::ConcurrentQueue<> m_meshingQueue;

void SpaceObjectManager::onDispose()
{

}

void SpaceObjectManager::init()
{
}

void SpaceObjectManager::update()
{
}

void SpaceObjectManager::enqueue(SpaceObjectChunk* chunk)
{

}