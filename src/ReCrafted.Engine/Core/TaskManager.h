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
    std::atomic<bool> m_running = false;
    uint m_sleepTime = 10;
    uint m_lastId = 0u;
    Lock m_createTaskLock = {};
    Lock m_globalLock = {};
    Lock m_callbackLock = {};
    moodycamel::ConcurrentQueue<Task> m_tasks;
    Array<Delegate<bool>> m_callbacks = {};
    Array<std::thread*> m_workerThreads = {};

private:
    void WorkerFunction();

private:
    void OnInit() override;
    void OnDispose() override;
    void OnLoad() override;
    void Update() override;

private:
    static bool CancelTask(uint taksId);
    static void QueueTask(const Task& task);
    static Task CreateTask(Delegate<void> function, Delegate<bool> callback);
    static Task CreateTask(ITask* customTask, void* userData);
};

#endif // TASKMANAGER_H
