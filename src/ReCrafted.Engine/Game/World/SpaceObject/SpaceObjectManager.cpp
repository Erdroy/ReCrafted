// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObjectManager.h"
#include "SpaceObjectOctreeNode.h"
#include "Core/Logger.h"
#include "Core/Lock.h"
#include "Core/Delegate.h"
#include "Core/Containers/concurrentqueue.h"
#include "Meshing/MarchingCubes/MCMesher.h"
#include "Platform/Platform.h"

SpaceObjectManager* Singleton<SpaceObjectManager>::m_instance;

ALIGN(8) struct queueItem
{
	SpaceObjectOctreeNode* node;
	ProcessMode::_enum mode;
	Delegate callback;
};

moodycamel::ConcurrentQueue<queueItem> m_loadingQueue;
Array<Delegate> m_callbacks;
Lock m_calbacksLock;

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
	Ptr<IVoxelMesher> mesher(new MCMesher);

	queueItem item;
	while(m_running)
	{
		if(!m_loadingQueue.try_dequeue(item))
		{
			Sleep(10);
			continue;
		}

		// populate or depopulate the queued node
		if(item.mode == ProcessMode::Populate)
			item.node->worker_populate(mesher.get());
		else
			item.node->worker_depopulate(mesher.get());

		// queue callback
		m_calbacksLock.lock();
		m_callbacks.add(item.callback);
		m_calbacksLock.unlock();
	}
}

void SpaceObjectManager::init()
{
	m_callbacks = {};

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
	// dispatch calbacks
	m_calbacksLock.lock();
	for(var & callback : m_callbacks)
		callback.Invoke();
	m_calbacksLock.unlock();
}

void SpaceObjectManager::enqueue(SpaceObjectOctreeNode* node, ProcessMode::_enum mode, Delegate callback)
{
	var item = queueItem();
	item.node = node;
	item.mode = mode;
	item.callback = callback;

	m_loadingQueue.enqueue(item);
}