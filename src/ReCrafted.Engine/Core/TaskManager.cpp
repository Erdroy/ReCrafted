// ReCrafted (c) 2016-2018 Always Too Late

#include "TaskManager.h"
#include "Delegate.h"
#include "Task.h"
#include "Platform/Platform.h"
#include "Logger.h"
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(TaskManager)

void TaskManager::WorkerFunction()
{
    Platform::SetThreadName("TaskManager Worker");

    Task task;
    while (m_running)
    {
        ScopeLock(m_globalLock);

        if (!m_tasks.try_dequeue(task))
        {
            Platform::Sleep(m_sleepTime);
            continue;
        }

        // run task
        task.Run();

        // queue callback
        m_callbackLock.LockNow();
        m_callbacks.Add(task.m_callback);
        m_callbackLock.UnlockNow();
    }
}

void TaskManager::OnInit()
{
    // run threads
    cvar maxThreads = Platform::CpuCount();

    Logger::LogInfo("Starting TaskManager workers.");
    m_running = true;
    for (var i = 0; i < maxThreads; i++)
    {
        m_workerThreads.Add(new std::thread([this]
        {
            var thread = RPMallocThread();
            WorkerFunction();
        }));
    }
}

void TaskManager::OnDispose()
{
    m_running = false;

    Logger::LogInfo("Waiting for TaskManager workers to exit...");

    // wait for threads to exit
    for (auto&& thread : m_workerThreads)
    {
        if (thread && thread->joinable())
            thread->join();

        SafeDelete(thread);
    }

    Logger::LogInfo("TaskManager workers exited.");
}

void TaskManager::OnLoad()
{
}

void TaskManager::Update()
{
    Profiler::BeginProfile("TaskManager dispatch");

    ScopeLock(m_callbackLock);

    for (var&& callback : m_callbacks)
        callback.Invoke();

    m_callbacks.Clear();

    Profiler::EndProfile();
}

bool TaskManager::CancelTask(uint taksId)
{
    rvar lock = m_instance->m_callbackLock;
    ScopeLock(lock);

    MISSING_CODE("Task cancelation is not implemented, yet!");
    return false;
}

void TaskManager::QueueTask(const Task& task)
{
    rvar lock = m_instance->m_callbackLock;
    ScopeLock(lock);

    m_instance->m_tasks.enqueue(task);
}

Task TaskManager::CreateTask(Delegate<void> function, Delegate<bool> callback)
{
    rvar lock = m_instance->m_createTaskLock;
    ScopeLock(lock);

    var task = Task();
    task.m_id = m_instance->m_lastId++;
    task.m_function = function;
    task.m_callback = callback;

    return task;
}

Task TaskManager::CreateTask(ITask* customTask, void* userData)
{
    rvar lock = m_instance->m_createTaskLock;
    ScopeLock(lock);

    var task = Task();
    task.m_id = m_instance->m_lastId++;
    task.m_customTask = customTask;
    task.m_userData = userData;

    return task;
}
