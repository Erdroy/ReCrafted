// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectManager.h"
#include "Common/Logger.h"
#include "Profiler/Profiler.h"

void VoxelObjectManager::WorkerFunction()
{
    Platform::SetThreadName("VoxelObject Worker");
    Logger::Log("VoxelObject worker thread startup");

    Profiler::InitThread("SpaceObject Worker");
    Profiler::BeginFrame();

    // TODO: Create mesher and physics shape cooker instance

    QueueItem item;
    while (m_running)
    {
        if (!m_loadingQueue.TryDequeue(item))
        {
            Profiler::EndFrame();
            Platform::Sleep(10); // TODO: This should be configurable
            Profiler::BeginFrame();
            continue;
        }

        Profiler::BeginCPUProfile("Frame");

        // Populate or depopulate the queued node
        switch (item.Mode)
        {
        case ProcessMode::Populate:
            //item.Node->WorkerPopulate(mesher);
            break;
        case ProcessMode::Depopulate:
            //item.Node->WorkerDepopulate(mesher);
            break;
        case ProcessMode::Rebuild:
            //item.Node->WorkerRebuild(mesher);
            break;
        default:;
        }

        // Queue callback
        m_callbackListLock.LockNow();
        m_callbackList.Add(item.Callback);
        m_callbackListLock.UnlockNow();

        Profiler::EndCPUProfile();
    }

    Profiler::FinishThread();
}

void VoxelObjectManager::InitializeWorkers()
{
    const auto maxThreads = Platform::CpuCount() - 1;

    // TODO: Game config - Voxel worker threads amount and priority

    Logger::Log("Starting {0} VoxelObject worker threads.", maxThreads);

    for (auto i = 0; i < maxThreads; i++)
    {
        m_workerThreads.Add(new std::thread([this] { WorkerFunction(); }));
    }
}

void VoxelObjectManager::DispatchCallbacks()
{
    CPU_PROFILE_FUNCTION(0);
    ScopeLock(m_callbackListLock);

    // Invoke all callbacks from the callback list.
    for (auto& callback : m_callbackList)
        callback.Invoke();

    // Clear callback list, as we have invoked all of the callbacks.
    m_callbackList.Clear();
}

void VoxelObjectManager::Initialize()
{
    m_running = true;

    // Initialize worker threads
    InitializeWorkers();
}

void VoxelObjectManager::Shutdown()
{
    Logger::Log("Waiting for VoxelObject worker threads to exit...");

    m_running = false;

    // wait for threads to exit
    for (auto&& thread : m_workerThreads)
    {
        if (thread && thread->joinable())
            thread->join();

        delete thread;
        thread = nullptr;
    }

    Logger::Log("VoxelObject worker threads exited.");
}

void VoxelObjectManager::OnUpdate()
{
    DispatchCallbacks();
}

void VoxelObjectManager::Enqueue(VoxelObjectOctree::Node* node, const ProcessMode mode, const Action<void>& callback)
{
    auto item = QueueItem();
    item.Node = node;
    item.Mode = mode;
    item.Callback = callback;

    // Add to processing queue
    GetInstance()->m_loadingQueue.Enqueue(item);
}
