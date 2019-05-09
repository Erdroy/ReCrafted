// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Lock.h"
#include "Common/List.h"
#include "Core/SubSystems/SubSystem.h"

#include <concurrentqueue.h>
#include <atomic>

/// <summary>
///     TaskManager class. Allows queueing task for processing in multi-threaded environment.
/// </summary>
class TaskManager final : public SubSystem<TaskManager>
{
    friend struct Task;

private:
    static const int InitialTaskCount = 32;

private:
    List<std::thread*> m_workerThreads = {};
    std::atomic<bool> m_running = false;
    uint m_sleepTime = 10;
    uint m_lastId = 0u;
    Lock m_globalLock = {};

    moodycamel::ConcurrentQueue<Task*> m_tasks;
    moodycamel::ConcurrentQueue<Task*> m_taskExecuteQueue;
    moodycamel::ConcurrentQueue<Task*> m_taskReleaseQueue;
    moodycamel::ConcurrentQueue<Task*> m_taskPool;

private:
    void WorkerFunction();
    Task* AcquireTask();
    void ReleaseTask(Task* task);

private:
    void Initialize() override;
    void Shutdown() override;
    void OnUpdate() override;

private:
    static void CleanupTask(Task* task);
    static bool CancelTask(uint taskId);
    static void QueueTask(Task* task);
    static Task* CreateTask(const Action<void>& function, const Action<void>& callback);
    static Task* CreateTask(ITask* customTask, void* userData);
};
