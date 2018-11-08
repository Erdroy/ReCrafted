// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObjectManager.h"
#include "SpaceObjectOctreeNode.h"
#include "Core/Lock.h"
#include "Core/Action.h"
#include "Core/Logger.h"
#include "Platform/Platform.h"

#include "Meshing/MarchingCubes/MarchingCubesMesher.h"
#include "Meshing/Transvoxel/TransvoxelMesher.h"

#include <concurrentqueue.h>
#include "Scripting/ScriptingEngine.h"
#include "Physics/PhysicsSystem.h"

SINGLETON_IMPL(SpaceObjectManager)

ALIGN(8) struct queueItem
{
    SpaceObjectOctreeNode* node;
    ProcessMode::_enum mode;
    Action<void> callback;
};

moodycamel::ConcurrentQueue<queueItem> m_loadingQueue;
Array<Action<void>> m_callbacks;
Lock m_calbacksLock;

void SpaceObjectManager::OnDispose()
{
    Logger::LogInfo("Waiting for SpaceObjectManager workers to exit...");

    m_running = false;

    // wait for threads to exit
    for (auto&& thread : m_workerThreads)
    {
        if (thread && thread->joinable())
            thread->join();

        SafeDelete(thread);
    }

    Logger::LogInfo("SpaceObjectManager workers exited.");
}

void SpaceObjectManager::WorkerFunction()
{
    Platform::SetThreadName("SpaceObjectManager Worker");

    // Attach this thread to mono
    ScriptingEngine::AttachCurrentThread();

    Logger::Log("SpaceObjectManager thread startup");

    // Create shape cooker
    Logger::Log("Creating Physics Shape Cooker");
    cvar shapeCooker = PhysicsSystem::Physics()->CreateCooker();
    shapeCooker->Initialize({});

    // Create mesher
    cvar mesher = new TransvoxelMesher();
    Logger::Log("Using {0} as planet chunk mesher.", mesher->GetName());
    mesher->Initialize(shapeCooker);

    // Run
    queueItem item;
    while (m_running)
    {
        if (!m_loadingQueue.try_dequeue(item))
        {
            Platform::Sleep(10);
            continue;
        }

        // Populate or depopulate the queued node
        switch (item.mode)
        {
        case ProcessMode::Populate:
            item.node->WorkerPopulate(mesher);
            break;
        case ProcessMode::Depopulate:
            item.node->WorkerDepopulate(mesher);
            break;
        case ProcessMode::Rebuild:
            item.node->WorkerRebuild(mesher);
            break;
        default: ;
        }

        // queue callback
        m_calbacksLock.LockNow();
        m_callbacks.Add(item.callback);
        m_calbacksLock.UnlockNow();
    }

    ScriptingEngine::DetachCurrentThread();
    PhysicsSystem::Physics()->ReleaseCooker(shapeCooker);
    delete mesher;
}

void SpaceObjectManager::Init()
{
    m_callbacks = {};

    m_running = true;

    // run threads
    cvar maxThreads = Platform::CpuCount() - 1;

    Logger::LogInfo("Starting {0} SpaceObjectManager workers.", maxThreads);

    for (var i = 0; i < maxThreads; i ++)
    {
        m_workerThreads.Add(new std::thread([this]
        {
            WorkerFunction();
        }));
    }
}

void SpaceObjectManager::Update()
{
    // dispatch calbacks
    m_calbacksLock.LockNow();
    for (var& callback : m_callbacks)
        callback.Invoke();

    m_callbacks.Clear();
    m_calbacksLock.UnlockNow();
}

void SpaceObjectManager::Enqueue(SpaceObjectOctreeNode* node, ProcessMode::_enum mode, Action<void> callback)
{
    var item = queueItem();
    item.node = node;
    item.mode = mode;
    item.callback = callback;

    m_loadingQueue.enqueue(item);
}
