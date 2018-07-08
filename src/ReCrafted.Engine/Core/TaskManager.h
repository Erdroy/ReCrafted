// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TASKMANAGER_H
#define TASKMANAGER_H

// includes
#include "ReCrafted.h"
#include "EngineComponent.h"
#include "Lock.h"
#include "Task.h"
#include "Containers/Array.h"

#include <concurrentqueue.h>
#include <atomic>

/**
 * \brief TaskManager class. Allows queueing task for processing in multi-threaded enviroment.
 */
class TaskManager : public EngineComponent<TaskManager>
{
    friend struct Task;

private:
    static const int InitialTaskCount = 32;

private:
    Array<std::thread*> m_workerThreads = {};
    std::atomic<bool> m_running = false;
    uint m_sleepTime = 10;
    uint m_lastId = 0u;
    Lock m_globalLock = {};
    moodycamel::ConcurrentQueue<Task*> m_taskExecuteQueue;
    moodycamel::ConcurrentQueue<Task*> m_taskReleaseQueue;
    moodycamel::ConcurrentQueue<Task*> m_taskPool;

private:
    void WorkerFunction();
    Task* AcquireTask();
    void ReleaseTask(Task* task);

private:
    void OnInit() override;
    void OnDispose() override;
    void OnLoad() override;
    void Update() override;

private:
    static void CleanupTask(Task* task);
    static bool CancelTask(uint taskId);
    static void QueueTask(Task* task);
    static Task* CreateTask(Delegate<void> function, Delegate<void> callback);
    static Task* CreateTask(ITask* customTask, void* userData);
};

#endif // TASKMANAGER_H
