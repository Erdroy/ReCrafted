// ReCrafted (c) 2016-2018 Always Too Late

#include "TaskManager.h"
#include "Delegate.h"
#include "Task.h"
#include "Platform/Platform.h"
#include "Logger.h"
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(TaskManager)

void TaskManager::worker_function()
{
    Platform::setThreadName("TaskManager Worker");

    Task task;
    while (m_running)
    {
        if (!m_tasks.try_dequeue(task))
        {
            Platform::sleep(m_sleepTime);
            continue;
        }

        // run task
        task.run();

        // queue callback
        m_callbackLock.lock();
        m_callbacks.add(task.m_callback);
        m_callbackLock.unlock();
    }
}

void TaskManager::onInit()
{
    // run threads
    cvar maxThreads = Platform::cpuCount();

    Logger::logInfo("Starting TaskManager workers.");
    m_running = true;
    for (var i = 0; i < maxThreads; i++)
    {
        m_workerThreads.add(new std::thread([this] {
            var thread = RPMallocThread();
            worker_function();
        }));
    }
}

void TaskManager::onDispose()
{
    m_running = false;

    Logger::logInfo("Waiting for TaskManager workers to exit...");

    // wait for threads to exit
    for (auto && thread : m_workerThreads)
    {
        if (thread && thread->joinable())
            thread->join();

        SafeDelete(thread);
    }

    Logger::logInfo("TaskManager workers exited.");
}

void TaskManager::onLoad()
{
}

void TaskManager::update()
{
    Profiler::beginProfile("TaskManager dispatch");

    ScopeLock(m_callbackLock);

    for(var && callback : m_callbacks)
        callback.Invoke();

    m_callbacks.clear();

    Profiler::endProfile();
}

Task TaskManager::createTask(Delegate<void> function, Delegate<bool> callback)
{
    var task = Task();
    task.m_timeQueue = Platform::getMiliseconds();
    task.m_id = m_instance->m_lastId++;
    task.m_function = function;
    task.m_callback = callback;
    
    m_instance->m_tasks.enqueue(task);

    return task;
}
