// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/ConcurrentQueue.h"
#include "Common/List.h"
#include "Common/Lock.h"
#include "Common/Action.h"
#include "Core/SubSystems/SubSystem.h"
#include "Voxels/VoxelObjectOctree.h"

/// <summary>
///     VoxelObjectManager class. Provides VoxelObject management functionality.
/// </summary>
API_CLASS(public, static, partial, noinherit)
class VoxelObjectManager final : public SubSystem<VoxelObjectManager>
{
    DELETE_COPY_MOVE(VoxelObjectManager);
    API_CLASS_BODY();

public:
    enum class ProcessMode
    {
        None,
        Populate,
        Depopulate,
        Rebuild
    };

private:
    ALIGN(8) struct QueueItem
    {
        VoxelObjectOctree::Node* Node = nullptr;
        ProcessMode Mode = ProcessMode::None;
        Action<void> Callback = nullptr;
    };

private:
    List<std::thread*> m_workerThreads = {};
    std::atomic<bool> m_running = false;

    ConcurrentQueue<QueueItem> m_loadingQueue;
    List<Action<void>> m_callbackList;
    Lock m_callbackListLock;

    List<VoxelObjectBase*> m_voxelObjects = {};
    Lock m_voxelObjectsLock = {};

private:
    void WorkerFunction();
    void InitializeWorkers();

    void DispatchCallbacks();

public:
    VoxelObjectManager() = default;
    ~VoxelObjectManager() = default;

protected:
    void Initialize() override;
    void Shutdown() override;

    void OnUpdate() override;

public:
    static void RegisterVoxelObject(VoxelObjectBase* voxelObject);
    static void UnregisterVoxelObject(VoxelObjectBase* voxelObject);

    /// <summary>
    ///     Queues given node for processing.
    /// </summary>
    static void Enqueue(VoxelObjectOctree::Node* node, ProcessMode mode, const Action<void>& callback);
};
