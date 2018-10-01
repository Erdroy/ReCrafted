// ReCrafted (c) 2016-2018 Always Too Late

#include "TaskManager.h"
#include "Task.h"
#include "Platform/Platform.h"
#include "Logger.h"
#include "Common/Profiler/Profiler.h"
#include "Scripting/ScriptingEngine.h"

SINGLETON_IMPL(TaskManager)

void TaskManager::WorkerFunction()
{
    Platform::SetThreadName("TaskManager Worker");
    ScriptingEngine::AttachCurrentThread();

    Logger::Log("TaskManager thread startup");

    Task* task;
    while (m_running)
    {
        ScopeLock(m_globalLock);

        if (!m_taskExecuteQueue.try_dequeue(task))
        {
            Platform::Sleep(m_sleepTime);
            continue;
        }

        // run task
        task->Run();

        // queue task for release
        m_taskReleaseQueue.enqueue(task);
    }

    ScriptingEngine::DetachCurrentThread();
}

Task* TaskManager::AcquireTask()
{
    Task* task;

    // Try to get task from the pool, 
    // if there is no any free task, create new one
    if (!m_taskPool.try_dequeue(task))
        task = new Task();

    return task;
}

void TaskManager::ReleaseTask(Task* task)
{
    // Invoke callback
    if(task->m_callback.IsValid())
        task->m_callback.Invoke();

    if (task->m_customTask)
        task->m_customTask->Finish();

    // Release 'Continue' task
    if (task->m_continueWith)
        ReleaseTask(task->m_continueWith);

    // Clean task
    CleanupTask(task);

    // Return task back to the pool
    m_taskPool.enqueue(task);
}

void TaskManager::CleanupTask(Task* task)
{
    delete task->m_customTask;

    // Cleanup 'continue with' task
    if (task->m_continueWith)
        CleanupTask(task->m_continueWith);

    task->m_id = 0u;
    task->m_timeQueue = 0.0f;
    task->m_timeStart = 0.0f;
    task->m_timeEnd = 0.0f;
    task->m_queued = false;
    task->m_completed = false;
    task->m_function = {};
    task->m_callback = {};
    task->m_customTask = nullptr;
    task->m_userData = nullptr;
    task->m_continueWith = nullptr;
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
            WorkerFunction();
        }));
    }

    // Initialize tasks
    for (var i = 0; i < InitialTaskCount; i++)
        m_taskPool.enqueue(new Task());
}

void TaskManager::OnDispose()
{
    m_running = false;

    Logger::LogInfo("Waiting for TaskManager workers to exit...");

    // wait for threads to exit
    for (rvar thread : m_workerThreads)
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

    Task* task;
    while(m_taskReleaseQueue.try_dequeue(task))
    {
        ReleaseTask(task);
    }

    Profiler::EndProfile();
}

bool TaskManager::CancelTask(uint taskId)
{
    MISSING_CODE("Task cancelation is not implemented, yet!");
    return false;
}

void TaskManager::QueueTask(Task* task)
{
    m_instance->m_taskExecuteQueue.enqueue(task);
}

Task* TaskManager::CreateTask(Action<void> function, Action<void> callback)
{
    var task = m_instance->AcquireTask();

    task->m_id = m_instance->m_lastId++;
    task->m_function = function;
    task->m_callback = callback;

    return task;
}

Task* TaskManager::CreateTask(ITask* customTask, void* userData)
{
    var task = m_instance->AcquireTask();

    task->m_id = m_instance->m_lastId++;
    task->m_customTask = customTask;
    task->m_userData = userData;

    return task;
}
