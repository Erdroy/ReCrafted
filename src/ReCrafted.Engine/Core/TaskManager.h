// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TASKMANAGER_H
#define TASKMANAGER_H

// includes
#include "ReCrafted.h"
#include "EngineComponent.h"
#include "Lock.h"
#include "Task.h"
#include "Containers/concurrentqueue.h"
#include "Containers/Array.h"

/**
 * \brief TaskManager class. Allows queueing task for processing in multi-threaded enviroment.
 */
class TaskManager : public EngineComponent<TaskManager>
{
    friend struct Task;

private:
    bool m_running = false;
    uint m_sleepTime = 10;
    uint m_lastId = 0u;
    Lock m_callbackLock = {};
    moodycamel::ConcurrentQueue<Task> m_tasks;
    Array<Delegate<bool>> m_callbacks = {};
    Array<std::thread*> m_workerThreads = {};

private:
    void worker_function();

private:
    void onInit() override;
    void onDispose() override;
    void onLoad() override;
    void update() override;

private:
    static Task createTask(Delegate<void> function, Delegate<bool> callback);
};

#endif // TASKMANAGER_H
