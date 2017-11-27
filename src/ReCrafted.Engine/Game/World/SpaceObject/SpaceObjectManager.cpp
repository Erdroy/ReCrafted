// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObjectManager.h"
#include "SpaceObjectChunk.h"
#include "Core/Logger.h"
#include "Core/Containers/concurrentqueue.h"
#include "Platform/Platform.h"

SpaceObjectManager* Singleton<SpaceObjectManager>::m_instance;

moodycamel::ConcurrentQueue<SpaceObjectChunk*> m_loadingQueue;

void SpaceObjectManager::onDispose()
{
	m_running = false;

	Logger::logInfo("Waiting for SpaceObjectManager workers to exit...");

	// wait for threads to exit
	for (auto && thread : m_workerThreads)
	{
		if (thread && thread->joinable())
			thread->join();

		SafeDelete(thread);
	}

	Logger::logInfo("SpaceObjectManager workers exited.");
}

void SpaceObjectManager::worker_function()
{
	SpaceObjectChunk* chunk = nullptr;

	while(m_running)
	{
		var found = m_loadingQueue.try_dequeue(chunk);
		
		if(!found || !chunk)
		{
			Sleep(10);
			continue;
		}

		// TODO: generate chunk
	}
}

void SpaceObjectManager::init()
{
	m_running = true;
	
	// run threads
	var maxThreads = Platform::cpuCount();

	Logger::logInfo("Starting SpaceObjectManager workers.");

	for(var i = 0; i < maxThreads; i ++)
	{
		m_workerThreads.add(new std::thread([this] {
			worker_function();
		}));
	}

	Logger::logInfo("SpaceObjectManager workers started.");
}

void SpaceObjectManager::update()
{

}

void SpaceObjectManager::enqueue(SpaceObjectChunk* chunk)
{
	m_loadingQueue.enqueue(chunk);
}