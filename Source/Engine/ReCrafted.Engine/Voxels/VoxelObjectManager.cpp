// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectManager.h"
#include "Common/Logger.h"
#include "Physics/ShapeCooker.h"
#include "Profiler/Profiler.h"
#include "Scripting/Object.h"
#include "VoxelObjectBase.h"
#include "Meshing/IVoxelMesher.h"
#include "Meshing/Transvoxel/TransvoxelMesher.h"
#include "Physics/PhysicsManager.h"

void VoxelObjectManager::WorkerFunction(int threadId)
{
    Platform::SetThreadName("VoxelObject Worker");

    // Attach this thread to mono
    ObjectManager::AttachThread();
    
    Logger::Log("VoxelObject worker thread startup");

    Profiler::InitThread("VoxelObject Worker");
    Profiler::BeginFrame();

    // Create shape cooker
    auto tolerances = PhysicsManager::GetDefaultTolerances();
    const auto shapeCooker = new ShapeCooker(PhysicsManager::GetFoundation(), tolerances);

    // Create mesher
    auto mesher = new TransvoxelMesher();
    mesher->Initialize(shapeCooker);

    QueueItem item;
    while (true)
    {
        if (!m_loadingQueue.TryDequeue(item))
        {
            // Exit thread when not running and there is nothing to work on
            if (!m_running) break;

            m_finishSignal.Emit(threadId);

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
            item.Node->WorkerPopulate(mesher);
            break;
        case ProcessMode::Rebuild:
            item.Node->WorkerRebuild(mesher);
            break;
        default:;
        }

        // Queue callback
        m_callbackListLock.LockNow();
        m_callbackList.Add(item.Callback);
        m_callbackListLock.UnlockNow();

        Profiler::EndCPUProfile();
    }

    // Detach this thread from mono
    ObjectManager::DetachThread();

    mesher->Clear();
    delete mesher;

    delete shapeCooker;
}

void VoxelObjectManager::InitializeWorkers()
{
    const auto maxThreads = Platform::CpuCount() - 1;

    // TODO: Game config - Voxel worker threads amount and priority

    Logger::Log("Starting {0} VoxelObject worker threads.", maxThreads);

    // Create wait signal
    m_finishSignal = Signal(uint16_t(maxThreads));

    for (auto i = 0; i < maxThreads; i++)
    {
        m_workerThreads.Add(new std::thread([this, i] { WorkerFunction(i); }));
    }
}

void VoxelObjectManager::UpdateVoxelObjects()
{
    CPU_PROFILE_FUNCTION(0);
    for (auto& voxelObject : m_voxelObjects)
    {
        voxelObject->Update();

        //voxelObject->DebugDraw();
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
    Logger::Log("Unloading VoxelObjects");
    for(auto& voxelObject : m_voxelObjects)
    {
        Object::DestroyNow(voxelObject);
    }

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
    CPU_PROFILE_FUNCTION(0);

    DispatchCallbacks();
    UpdateVoxelObjects();
}

void VoxelObjectManager::WaitForFinish()
{
    MAIN_THREAD_ONLY();

    Logger::Log("Waiting for VoxelObjectManager to finish it's work...");

    // Wait for finish
    m_finishSignal.Reset();
    m_finishSignal.WaitMultiple();

    // Force calling all events if main thread
    ScopeLock(m_callbackListLock);

    // Invoke all callbacks from the callback list.
    for (auto& callback : m_callbackList)
        callback.Invoke();

    // Clear callback list, as we have invoked all of the callbacks.
    m_callbackList.Clear();
}

void VoxelObjectManager::RegisterVoxelObject(VoxelObjectBase* voxelObject)
{
    GetInstance()->m_voxelObjectsLock.LockNow();
    GetInstance()->m_voxelObjects.Add(voxelObject);
    GetInstance()->m_voxelObjectsLock.UnlockNow();
}

void VoxelObjectManager::UnregisterVoxelObject(VoxelObjectBase* voxelObject)
{
    GetInstance()->m_voxelObjectsLock.LockNow();
    GetInstance()->m_voxelObjects.Remove(voxelObject);
    GetInstance()->m_voxelObjectsLock.UnlockNow();
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
