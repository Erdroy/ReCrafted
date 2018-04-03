// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObjectManager.h"
#include "SpaceObjectOctreeNode.h"
#include "Core/Lock.h"
#include "Core/Delegate.h"
#include "Core/Logger.h"
#include "Core/Containers/concurrentqueue.h"
#include "Meshing/MarchingCubes/MCMesher.h"
#include "Platform/Platform.h"

SINGLETON_IMPL(SpaceObjectManager)

ALIGN(8) struct queueItem
{
	SpaceObjectOctreeNode* node;
	ProcessMode::_enum mode;
    Delegate<void> callback;
};

moodycamel::ConcurrentQueue<queueItem> m_loadingQueue;
Array<Delegate<void>> m_callbacks;
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
    Platform::setThreadName("SpaceObjectManager Worker");

    var thread = RPMallocThread();

	Ptr<IVoxelMesher> mesher(new MCMesher);

	queueItem item;
	while(m_running)
	{
		if(!m_loadingQueue.try_dequeue(item))
		{
			Platform::sleep(10);
			continue;
		}

        // populate or depopulate the queued node
	    switch (item.mode) { 
	        case ProcessMode::Populate:
                item.node->worker_populate(mesher.get()); 
	        break;
            case ProcessMode::Depopulate:
                item.node->worker_depopulate(mesher.get());
	        break;
            case ProcessMode::Rebuild:
                item.node->worker_rebuild(mesher.get());
	        break;
            default:;
	    }

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
	cvar maxThreads = Platform::cpuCount();

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

	m_callbacks.clear();
	m_calbacksLock.unlock();
}

void SpaceObjectManager::enqueue(SpaceObjectOctreeNode* node, ProcessMode::_enum mode, Delegate<void> callback)
{
	var item = queueItem();
	item.node = node;
	item.mode = mode;
	item.callback = callback;

	m_loadingQueue.enqueue(item);
}